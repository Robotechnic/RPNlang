#include "interpreter/interpreter.hpp"

Interpreter::Interpreter() :
	lastValue(nullptr),
	context(std::make_shared<Context>("main", "<stdin>"))
{}

Interpreter::Interpreter(ContextPtr ctx) : 
	lastValue(nullptr),
	context(ctx)
{}

Interpreter::~Interpreter() {
	Value::deleteValue(&this->lastValue, Value::INTERPRETER);
	this->memory.clear();
}

bool Interpreter::interpretFile(std::string_view fileName, std::string &errorString, bool isModule) {
	std::ifstream file;
	if (!openFile(file, fileName, errorString)) {
		return false;
	}

	bool error = false;
	std::queue<Token*> tokens;
	std::string instruction;
	int line = 0;
	ExpressionResult result;
	while (!error && std::getline(file, instruction)) {
		line++;
		result = Lexer::tokenize(line, instruction, tokens, this->context);
		if (result.error()) {
			result.display();
			error = true;
		}
		tokens.push(new StringToken(line, instruction.size(), TOKEN_TYPE_END_OF_LINE, "\n"));
	}
	file.close();
	
	if (error)
		return false;				

	Lexer lexer(tokens, this->context);
	result = lexer.lex();
	if (result.error()) {
		result.display();
		return false;
	}

	Value::deleteValue(&this->lastValue, Value::INTERPRETER);
	result = this->interpret(lexer.getBlocks());
	if (result.error()) {
		result.display();
		return false;
	}
	if (this->context->hasValue("main") && this->context->getType() != CONTEXT_TYPE_MODULE && this->context->getType() != CONTEXT_TYPE_BUILTIN_MODULE) {
		Function* val = static_cast<Function*>(this->context->getValue("main"));
		TextRange mainRange = val->getRange();
		if (val->getType() == FUNCTION) {
			const RPNFunction* func = val->getValue();
			RPNFunctionArgs args;
			RPNFunctionResult mainResult = func->call(args, mainRange, this->context);
			Value::deleteValue(&mainResult.second, Value::INTERPRETER);
			if (std::get<0>(mainResult).error()) {
				std::get<0>(mainResult).display();
				return false;
			}
		}
	}

	
	if (isModule)
		this->context->takeOwnership();
	else
		this->context->clear();

	return true;
}

/**
 * @brief take a string line, parse it and interpret the result
 * 
 * @param line the line to interpret
 * @param lineNumber the line number of the line
 * @return ExpressionResult status of the interpretation
 */
ExpressionResult Interpreter::interpretLine(std::string_view line, int lineNumber) {
	std::queue<Token*> tokens;
	ExpressionResult result = Lexer::tokenize(lineNumber, std::string(line), tokens, this->context);
	if (result.error()) return result;

	Lexer lexer(tokens, this->context);
	result = lexer.lex();
	if (result.error()) return result;

	Value::deleteValue(&this->lastValue, Value::INTERPRETER);
	result = this->interpret(lexer.getBlocks());
	if (result.error()) return result;

	return this->checkMemory();
}

/**
 * @brief return the last value remaining in the interpreter memory
 * 
 * @return Value the last value in the stack
 */
Value *Interpreter::getLastValue() const {
	return this->lastValue;
}

/**
 * @brief get the minimum and maximum text range of value list
 * 
 * @param values values that the text range will be calculated from
 * @return TextRange text range of the tokens list
 */
TextRange Interpreter::mergeRanges(const std::vector<Value*> &values) {
	if (values.size() == 0) return TextRange();
	
	TextRange range = values[0]->getRange();
	for (const Value *value : values) {
		range.merge(value->getRange());
	}
	
	return range;
}

/**
 * @brief check if memory contains less than 1 value at the end of the interpretation
 * 
 * @return ExpressionResult error if memory contains more than 1 value
 */
ExpressionResult Interpreter::checkMemory() {	
	if (this->memory.empty()) {
		Value::deleteValue(&this->lastValue, Value::INTERPRETER);
		this->lastValue = None::empty();
		return ExpressionResult();
	}

	if (this->memory.size() > 1) {
		int size = this->memory.size();
		Value *last = this->memory.pop();
		this->memory.clear(1);
		ExpressionResult result(
			"To much remaining values in the memory (" + std::to_string(size) + ")",
			last->getRange().merge(this->memory.top()->getRange()),
			this->context
		);
		Value::deleteValue(&last, Value::INTERPRETER);
		Value::deleteValue(&this->memory.pop(), Value::INTERPRETER);

		return result;
	}

	Value::deleteValue(&this->lastValue, Value::INTERPRETER);
	return this->memory.popVariableValue(this->lastValue, this->context);
}

/**
 * @brief interpret a list of blocks
 * 
 * @param blocks the list of blocks to interpret
 * @param free if true, free the blocks after interpretation
 * @return ExpressionResult status of the interpretation
 */
ExpressionResult Interpreter::interpret(BlockQueue &blocks) {
	ExpressionResult result;
	BlockQueueIterator it = blocks.begin();
	while (it && !result.stopInterpret()) {
		BaseBlock *block = *(it++);
		if (block->getType() == LINE_BLOCK) {
			Line *l = static_cast<Line*>(block);
			if (it && (*it)->getType() == CODE_BLOCK) {
				result = this->interpretBlock(*l, *static_cast<CodeBlock*>(*(it++)));
			} else {
				result = interpretLine(*l);
			}
		} else if (block->getType() == FUNCTION_BLOCK) {
			FunctionBlock *f = static_cast<FunctionBlock*>(block);
			this->context->setValue(f->getName(), new Function(
				f->getFunction(),
				f->lastRange(),
				Value::CONTEXT_VARIABLE
			));
		} else {
			result = ExpressionResult(
				"Lexer didn't to its job corectly ;(",
				static_cast<CodeBlock*>(block)->getKeyword()->getRange(),
				this->context
			);
		}
	}
	return result;
}

/**
 * @brief interpret a line of tokens
 * 
 * @param line the line to interpret
 * @return ExpressionResult status of the interpretation
 */
ExpressionResult Interpreter::interpretLine(Line &line, bool clearMemory) {
	ExpressionResult result;
	LineIterator it = line.begin();
	while (!result.stopInterpret() && it) {
		switch ((*it)->getType()) {
			case TOKEN_TYPE_VALUE:
			case TOKEN_TYPE_LITERAL:
				this->memory.push(static_cast<ValueToken*>(*it)->getValue());
				break;
			case TOKEN_TYPE_OPERATOR:
			case TOKEN_TYPE_BOOLEAN_OPERATOR:
				result = this->interpretOperator(*it);
				break;
			case TOKEN_TYPE_FUNCTION_CALL:
			case TOKEN_TYPE_MODULE_FUNCTION_CALL:
				result = this->interpretFunctionCall(*it);
				break;
			case TOKEN_TYPE_FSTRING:
				result = this->interpretFString(static_cast<FStringToken*>(*it));
				break;
			case TOKEN_TYPE_ASSIGNMENT:
				result = this->interpretAssignment(*it);
				break;
			case TOKEN_TYPE_KEYWORD:
				result = this->interpretKeyword(*it);
				break;
			case TOKEN_TYPE_VALUE_TYPE:
				result = this->interpretValueType(*it);
				break;
			default:
				result = ExpressionResult(
					"Unknow token (TODO)",
					(*it)->getRange(),
					this->context
				);
				break;
		}
		it++;
	}
	if (result.error()) return result;
	if (clearMemory) {
		ExpressionResult memory = this->checkMemory();
		if (memory.error()) {
			return memory;
		}
	}
	return result;
}

/**
 * @brief interpret a block of code
 * 
 * @param line the line which contains the block parameters
 * @param block the block to interpret
 * @return ExpressionResult status of the interpretation
 */
ExpressionResult Interpreter::interpretBlock(Line &line, CodeBlock &block) {
	const std::string type = block.getKeyword()->getStringValue();
	if (type == "if") {
		return this->interpretIf(line, block);
	} else if (type == "while") {
		return this->interpretWhile(line, block);
	} else if (type == "for") {
		return this->interpretFor(line, block);
	} else if (type == "try") {
		return this->interpretTry(line, block);
	} else {
		return ExpressionResult(
			"Unknow block type",
			block.getKeyword()->getRange(),
			this->context
		);
	}
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretFString(const FStringToken *token) {
	ExpressionResult sizeOk = this->memory.sizeExpected(
		token->getParts().size() - 1,
		"missing fString parameters expected " + std::to_string(token->getParts().size() - 1) + 
								   " but got " + std::to_string(this->memory.size()),
		token->getRange(),
		this->context
	);
	TextRange range = token->getRange();
	if (sizeOk.error()) return sizeOk;
	std::string str;
	Value *value;
	for (size_t i = token->size() - 1; i > 0; i--) {
		ExpressionResult result = this->memory.popVariableValue(value, this->context);
		if (result.error()) return result;
		str = value->getStringValue() + token->at(i) + str;
		if (i == 1) range.merge(value->getRange());
		Value::deleteValue(&value, Value::INTERPRETER);
	}
	str = token->getParts().at(0) + str;
	this->memory.push(new String(
		str,
		range,
		Value::INTERPRETER
	));
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretOperator(const Token *operatorToken) {
	ExpressionResult result = this->memory.sizeExpected(
		2,
		"Not enough values for operator " + operatorToken->getStringValue(), 
		operatorToken->getRange(),
		this->context
	);
	if (result.error()) return result;
	Value *right, *left;
	result = this->memory.popVariableValue(right, this->context);
	if (result.error()) return result;
	result = this->memory.popVariableValue(left, this->context);
	if (result.error()) {
		Value::deleteValue(&right, Value::INTERPRETER);
		return result;
	}
	operatorResult opResult = left->applyOperator(right, operatorToken, this->context);
	Value::deleteValue(&right, Value::INTERPRETER);
	if (right != left)
		Value::deleteValue(&left, Value::INTERPRETER);
	if (std::get<0>(opResult).error()) {
		return std::get<0>(opResult);
	}
	
	this->memory.push(opResult.second);
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretKeyword(const Token *keywordToken) {
	const std::string keyword = keywordToken->getStringValue();
	if (keyword == "break") {
		return ExpressionResult(ExpressionResult::BREAK);
	} else if (keyword == "continue") {
		return ExpressionResult(ExpressionResult::CONTINUE);
	}  else if (keyword == "return") {
		return ExpressionResult(ExpressionResult::RETURN);
	} else {
		return ExpressionResult(
			"Unknow keyword " + keyword,
			keywordToken->getRange(),
			this->context
		);
	}
}

ExpressionResult Interpreter::interpretValueType(const Token *typeToken) {
	ValueType type = static_cast<const TypeToken *>(typeToken)->getValueType();
	ExpressionResult result = this->memory.sizeExpected(
		1,
		"Not enough values for type conversion",
		typeToken->getRange(),
		this->context
	);
	if (result.error()) return result;
	if (type == LIST && !this->memory.top()->isCastableTo(LIST)) {
		return this->interpretList(typeToken);
	}

	Value *top;
	result = this->memory.popVariableValue(top, this->context);
	if (result.error()) return result;

	if (!top->isCastableTo(type)) {
		return ExpressionResult(
			"Can't cast " + top->getStringType() + " to " + Value::stringType(type),
			typeToken->getRange(),
			this->context
		);
	}
	
	Value *value = top->to(type, Value::INTERPRETER);
	Value::deleteValue(&top, Value::INTERPRETER);
	this->memory.push(value);
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretAssignment(const Token *operatorToken) {
	ExpressionResult result = this->memory.sizeExpected(
		2,
		"Not enough values for operator " + operatorToken->getStringValue(), 
		operatorToken->getRange(),
		this->context
	);
	bool copy = this->memory.top()->getType() == VARIABLE || this->memory.top()->getType() == PATH;
	if (result.error()) return result;
	Value *left;
	result = this->memory.popVariableValue(left, this->context);
	if (result.error()) return result;
	if (this->memory.top()->getType() != VARIABLE) {
		return ExpressionResult(
			"Can't assign value to a non variable",
			this->memory.top()->getRange(),
			this->context
		);
	}
	Value *hold;
	this->context->setValue(this->memory.top(), copy ? left->copy() : left, &hold);
	if (this->lastValue == hold)
		this->lastValue = nullptr;
	
	return result;
}

ExpressionResult Interpreter::getFunction(const Token *functionToken, const RPNFunction *&function) {
	if (builtins::builtinFunctions.contains(functionToken->getStringValue())) {
		function = &builtins::builtinFunctions.at(functionToken->getStringValue());
	} else {
		Value *value;
		ExpressionResult result;
		if (functionToken->getType() == TOKEN_TYPE_FUNCTION_CALL) {
			result = this->context->getValue(functionToken, value);
		} else {
			result = Module::getModuleValue(functionToken, value, context);
		}
		if (result.error()) return result;
		if (value->getType() != FUNCTION)
			return ExpressionResult(
				functionToken->getStringValue() + " is not a function",
				functionToken->getRange(),
				this->context
			);
		function = static_cast<const Function*>(value)->getValue();
	}
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretFunctionCall(const Token *functionToken) {
	const RPNFunction *function;
	ExpressionResult result = this->getFunction(functionToken, function);
	if (result.error()) return result;
	result = this->memory.sizeExpected(
		function->getArgumentsCount(),
		"Invalid number of arguments for function " + functionToken->getStringValue() +
		" got " + std::to_string(this->memory.size()) +
		" but expected " + std::to_string(function->getArgumentsCount()) + " arguments",
		functionToken->getRange(),
		this->context
	);
	if (result.error()) return result;

	std::vector<Value*> arguments;
	Value *value;
	for (size_t i = 0; i < function->getArgumentsCount(); i++) {
		result = this->memory.popVariableValue(value, this->context);
		if (result.error()) return result;
		arguments.insert(arguments.begin(), value);
	}
	
	RPNFunctionResult callResult;
	if (functionToken->getType() == TOKEN_TYPE_FUNCTION_CALL) {
		callResult = function->call(arguments, functionToken->getRange(), this->context);
	} else {
		ContextPtr ctx;
		ExpressionResult result = Module::getModuleContext(functionToken, this->context, ctx);
		if (result.error()) return result;
		callResult = function->call(arguments, functionToken->getRange(), ctx);
	}
	for (Value *value : arguments)
		Value::deleteValue(&value, Value::INTERPRETER);
	if (std::get<0>(callResult).error()) {
		Value::deleteValue(&callResult.second, Value::INTERPRETER);
		return std::get<0>(callResult);
	}
	this->memory.push(callResult.second);
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretIf(Line &line, CodeBlock &block) {
	ExpressionResult result = this->interpretLine(line);
	if (result.error()) return result;
	Value *condition = this->lastValue->to(BOOL);
	if (static_cast<Bool*>(condition)->getValue()) {
		Value::deleteValue(&condition, Value::INTERPRETER);
		return this->interpret(block.getBlocks());
	}
	Value::deleteValue(&condition, Value::INTERPRETER);
	if (block.getNext() != nullptr)
		return this->interpret(block.getNext()->getBlocks());
	
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretWhile(Line &line, CodeBlock &block) {
	ExpressionResult result = this->interpretLine(line);
	if (result.error()) return result;
	Value *condition = this->lastValue->to(BOOL);
	while (static_cast<Bool*>(condition)->getValue()) {
		Value::deleteValue(&condition, Value::INTERPRETER);
		block.reset();
		result = this->interpret(block.getBlocks());
		if (result.error()) return result;
		if (result.breakingLoop()) return ExpressionResult();
		result = this->interpretLine(line);
		if (result.error()) return result;
		condition = this->lastValue->to(BOOL);
	}
	Value::deleteValue(&condition, Value::INTERPRETER);
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretFor(Line &line, CodeBlock &block) {
	ExpressionResult result = this->interpretLine(line, false);
	if (result.error()) return result;
	result = this->memory.sizeExpected(
		4,
		"Invalid number of arguments for for loop, expected 4 but got " + std::to_string(this->memory.size()),
		block.getRange(),
		this->context
	);
	if (result.error()) return result;
	std::vector<Int*> forParams;
	Value *param;
	for (int i = 0; i < 3; i++) {
		result = this->memory.popVariableValue(param, this->context);
		if (result.error()) return result;
		if (param->getType() != INT) {
			return ExpressionResult(
				"Invalid type for for loop parameter, expected int but got " + param->getStringType(),
				block.getRange(),
				this->context
			);
		}
		forParams.emplace(forParams.begin(), static_cast<Int*>(param));
	}

	Value *variable = this->memory.pop();
	if (variable->getType() != VARIABLE) {
		TextRange range = variable->getRange();
		Value::deleteValue(&variable, Value::INTERPRETER);
		return ExpressionResult(
			"Can't assign value to a non variable",
			range,
			this->context
		);
	}
	Int zero {0, TextRange(), Value::PARENT_FUNCTION};
	CPPInterface step {forParams.at(2)};
	if (step == &zero) {
		Value::deleteValue(&variable, Value::INTERPRETER);
		return ExpressionResult(
			"Step can't be 0",
			forParams.at(2)->getRange(),
			this->context
		);
	}

	CPPInterface i {forParams.at(0)};
	while (!result.breakingLoop() && ((step > &zero && i < forParams.at(1)) || (step < &zero && i > forParams.at(1)))) {
		this->context->setValue(
			variable->getStringValue(),
			i.getValue()
		);
		result = this->interpret(block.getBlocks());
		if (result.error()) return result;
		i += step;
	}
	Value::deleteValue(&i.getValue(), Value::INTERPRETER);
	Value::deleteValue(&variable, Value::INTERPRETER);
	for (Value *param : forParams)
		Value::deleteValue(&param, Value::INTERPRETER);
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretTry(Line &line, CodeBlock &block) {
	if (line.size() != 1) {
		return ExpressionResult(
			"Invalid number of arguments for try, expected 1 but got " + std::to_string(line.size()),
			line.lastRange(),
			this->context
		);
	}
	if (line.top()->getType() != TOKEN_TYPE_LITERAL) {
		return ExpressionResult(
			"Invalid type for try, expected literal but got " + line.top()->getStringType(),
			line.top()->getRange(),
			this->context
		);
	}

	// try
	ExpressionResult result = this->interpret(block.getBlocks());
	if (block.getNext() == nullptr)
		return result;
	
	// finally
	if (!result.error() || block.getNext()->getKeyword()->getStringValue() == "finally")
		return this->interpret(block.getNext()->getBlocks());
	
	// catch
	if (block.getNext()->getKeyword()->getStringValue() != "catch")
		return result;
	
	this->context->setValue(
		line.top()->getStringValue(),
		new String(result.getErrorMessage(), line.top()->getRange(), Value::CONTEXT_VARIABLE)
	);

	result = this->interpret(block.getNext()->getBlocks());
	if (result.error()) return result;

	// finally
	if (block.getNext()->getNext() != nullptr)
		return this->interpret(block.getNext()->getNext()->getBlocks());
	

	return result;
}

ExpressionResult Interpreter::interpretList(const Token *keywordToken) {
	ExpressionResult result = this->memory.sizeExpected(
		1,
		"Expected list size before list keyword",
		keywordToken->getRange(),
		this->context
	);
	if (result.error()) return result;
	Int *size = static_cast<Int*>(this->memory.pop());
	if (size->getValue() < 0) {
		return ExpressionResult(
			"List size must be positive",
			size->getRange(),
			this->context
		);
	}
	result = this->memory.sizeExpected(
		size->getValue(),
		"Not enough values to create list, expected " + std::to_string(size->getValue()) + " but got " + std::to_string(this->memory.size()),
		keywordToken->getRange().merge(size->getRange()),
		this->context
	);
	if (result.error()) return result;
	TextRange range = keywordToken->getRange().merge(size->getRange());
	std::vector<Value*> values;
	Value *value;
	for (int i = 0; i < size->getValue(); i++) {
		result = this->memory.popVariableValue(value, this->context);
		if (result.error()) return result;
		value->setOwner(Value::OBJECT_VALUE);
		values.emplace(values.begin(), value);
		range.merge(values.at(0)->getRange());
	}
	this->memory.push(new List(values, range, Value::INTERPRETER));
	return ExpressionResult();
}
