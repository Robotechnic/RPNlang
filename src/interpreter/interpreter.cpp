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
	if (
		this->context->hasValue("main") && 
		this->context->getType() != CONTEXT_TYPE_MODULE && 
		this->context->getType() != CONTEXT_TYPE_BUILTIN_MODULE
	) {
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
		TextRange range = this->memory.top()->getRange();
		this->memory.clear(1);
		ExpressionResult result(
			"To much remaining values in the memory (" + std::to_string(size) + ")",
			range.merge(this->memory.top()->getRange()),
			this->context
		);
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
				static_cast<CodeBlock*>(block)->getRange(),
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
			case TOKEN_TYPE_PATH:
			case TOKEN_TYPE_STRUCT_ACCESS:
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
			case TOKEN_TYPE_STRUCT_NAME:
				result = this->interpretStruct(*it);
				break;
			default:
				result = ExpressionResult(
					"Unknow token (TODO) : " + Token::stringType((*it)->getType()),
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
	switch (block.getKeyword()) {
		case KEYWORD_IF:
			return this->interpretIf(line, block);
		case KEYWORD_WHILE:
			return this->interpretWhile(line, block);
		case KEYWORD_FOR:
			return this->interpretFor(line, block);
		case KEYWORD_TRY:
			return this->interpretTry(line, block);
		default:
			return ExpressionResult(
				"Unknow block type",
				block.getRange(),
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
	switch(static_cast<const KeywordToken *>(keywordToken)->getKeyword()) {
		case KEYWORD_BREAK:
			return ExpressionResult(ExpressionResult::BREAK);
		case KEYWORD_CONTINUE:
			return ExpressionResult(ExpressionResult::CONTINUE);
		case KEYWORD_RETURN:
			return ExpressionResult(ExpressionResult::RETURN);
		default:
			return ExpressionResult(
				"Unknow keyword " + keywordToken->getStringValue(),
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
	if (type == LIST && !Value::isCastableTo(this->memory.top()->getType(), LIST)) {
		return this->interpretList(typeToken);
	}

	Value *top;
	result = this->memory.popVariableValue(top, this->context);
	if (result.error()) return result;

	if (!Value::isCastableTo(top->getType(), type)) {
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
		"Not enough values for assignement",
		operatorToken->getRange(),
		this->context
	);
	bool copy = this->memory.top()->getType() == VARIABLE || 
				this->memory.top()->getType() == PATH ||
				this->memory.top()->getOwner() == Value::OBJECT_VALUE;
	if (result.error()) return result;
	Value *left;
	result = this->memory.popVariableValue(left, this->context);
	if (result.error()) return result;
	if (this->memory.top()->getType() != VARIABLE && this->memory.top()->getType() != STRUCT_ACCESS) {
		return ExpressionResult(
			"Can't assign value to a non variable",
			this->memory.top()->getRange(),
			this->context
		);
	}
	Value *hold = nullptr;
	if (this->memory.top()->getType() == VARIABLE) {
		this->context->setValue(this->memory.top(), copy ? left->copy() : left, &hold);
	} else {
		Path *path = static_cast<Path *>(this->memory.top());
		Value *structValue;
		result = Struct::getStruct(path, structValue, this->context);
		if (result.error()) return result;
		result = static_cast<Struct *>(structValue)->setMember(
			path,
			copy ? left->copy() : left,
			this->context,
			&hold
		);
		if (result.error()) return result;
	}
	if (this->lastValue == hold)
		this->lastValue = nullptr;
	
	return result;
}

ExpressionResult Interpreter::getFunction(const Value *functionName, const RPNFunction *&function) {
	if (functionName->getType() != PATH && builtins::builtinFunctions.contains(functionName->getStringValue())) {
		function = &builtins::builtinFunctions.at(functionName->getStringValue());
	} else {
		Value *value;
		ExpressionResult result;
		if (functionName->getType() == VARIABLE) {
			result = this->context->getValue(functionName, value);
		} else {
			result = Module::getModuleValue(functionName, value, context);
		}
		if (result.error()) return result;
		if (value->getType() != FUNCTION)
			return ExpressionResult(
				functionName->getStringValue() + " is not a function",
				functionName->getRange(),
				this->context
			);
		function = static_cast<const Function*>(value)->getValue();
	}
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretFunctionCall(Token *functionToken) {
	const RPNFunction *function;
	Value *functionName = static_cast<ValueToken *>(functionToken)->getValue();
	ExpressionResult result = this->getFunction(functionName, function);
	if (result.error()) return result;
	result = this->memory.sizeExpected(
		function->getArgumentsCount(),
		"Invalid number of arguments for function " + functionName->getStringValue() +
		" got " + std::to_string(this->memory.size()) +
		" but expected " + std::to_string(function->getArgumentsCount()) + " arguments",
		functionName->getRange(),
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
		callResult = function->call(arguments, functionName->getRange(), this->context);
	} else {
		ContextPtr ctx;
		ExpressionResult result = Module::getModuleContext(functionName, this->context, ctx);
		if (result.error()) return result;
		callResult = function->call(arguments, functionName->getRange(), ctx);
	}
	if (arguments.size() > 0)
		callResult.second->setVariableRange(TextRange::merge(functionName->getRange(), arguments.front()->getRange()));
	else
		callResult.second->setVariableRange(functionName->getRange());
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
		if (result.returnValue()) return result;
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
	while (!result.breakingLoop() && !result.returnValue() && 
			((step > &zero && i < forParams.at(1)) || 
			 (step < &zero && i > forParams.at(1)))
		  ) {
		this->context->setValue(
			variable->getStringValue(),
			i.getValue()
		);
		result = this->interpret(block.getBlocks());
		if (result.error()) return result;
		if (!result.breakingLoop())
			i += step;
	}
	if (i.getValue() != forParams.at(0)) {
		Value::deleteValue(&i.getValue(), Value::INTERPRETER);
	}
	Value::deleteValue(&variable, Value::INTERPRETER);
	for (Value *param : forParams) {
		Value::deleteValue(&param, Value::INTERPRETER);
	}
	if (result.returnValue()) return result;
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
	if (!result.error()) {
		if (block.getNext()->getKeyword() == KEYWORD_FINALLY)
			return this->interpret(block.getNext()->getBlocks());
		return result;
	}
	
	// catch
	if (block.getNext()->getKeyword() != KEYWORD_CATCH) {
		if (block.getNext()->getKeyword() == KEYWORD_FINALLY)
			return this->interpret(block.getNext()->getBlocks());
		return ExpressionResult();
	}
	
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
		"Not enough values to create list, expected " + std::to_string(size->getValue()) + 
		" but got " + std::to_string(this->memory.size()),
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
		if (value->getOwner() == Value::OBJECT_VALUE)
			value = value->copy();
		value->setOwner(Value::OBJECT_VALUE);
		values.emplace(values.begin(), value);
		range.merge(values.at(0)->getRange());
	}
	this->memory.push(new List(values, range, Value::INTERPRETER));
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretStruct(const Token *keywordToken) {
	std::string name = keywordToken->getStringValue();
	if (!Struct::structExists(name)) {
		return ExpressionResult(
			"Undefined struct name : " + std::string(name),
			keywordToken->getRange(),
			this->context
		);
	};
	size_t count = Struct::getStructMembersCount(name);
	ExpressionResult result = this->memory.sizeExpected(
		count,
		"Not enough values to create struct, expected " + std::to_string(count) +
		" but got " + std::to_string(this->memory.size()),
		keywordToken->getRange(),
		this->context
	);
	if (result.error()) return result;
	std::vector<Value *> members(count, nullptr);
	TextRange range = keywordToken->getRange();
	for (size_t i = 0; i < count; i++) {
		result = this->memory.popVariableValue(members.at(count - i - 1), this->context);
		if (result.error()) return result;
		if (members.at(count - i - 1)->getOwner() == Value::OBJECT_VALUE)
			members.at(count - i - 1) = members.at(count - i - 1)->copy();
		members.at(count - i - 1)->setOwner(Value::OBJECT_VALUE);
	}
	range.merge(members.at(0)->getRange());
	Struct *s = new Struct(range, name, Value::INTERPRETER);
	result = s->setMembers(members, this->context);
	if (result.error()) return result;
	this->memory.push(s);
	return ExpressionResult();
}