#include "interpreter/interpreter.hpp"

Interpreter::Interpreter() :
	lastValue(nullptr),
	context(new Context("main", "<stdin>"))
{}

Interpreter::Interpreter(Context *ctx) : 
	lastValue(nullptr),
	context(ctx)
{}

Interpreter::~Interpreter() {
	Value::deleteValue(&this->lastValue);
	this->memory.clear();
}

bool Interpreter::interpretFile(std::string fileName, std::string &errorString) {
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
	
	if (error) return false;				

	Lexer lexer(tokens, this->context);
	result = lexer.lex();
	if (result.error()) {
		result.display();
		return false;
	}

	Value::deleteValue(&this->lastValue);
	result = this->interpret(lexer.getBlocks());
	if (result.error()) {
		result.display();
		return false;
	}
	if (this->context->hasValue("main") && this->context->getType() != CONTEXT_TYPE_MODULE) {
		Function* val = static_cast<Function*>(this->context->getValue("main"));
		TextRange mainRange = val->getRange();
		if (val->getType() == FUNCTION) {
			const RPNFunction* func = val->getValue();
			RPNFunctionResult mainResult = func->call({}, mainRange, this->context);
			delete std::get<1>(mainResult);
			if (std::get<0>(mainResult).error()) {
				std::get<0>(mainResult).display();
				return false;
			}
		}
	}

	return true;
}

/**
 * @brief take a string line, parse it and interpret the result
 * 
 * @param line the line to interpret
 * @param lineNumber the line number of the line
 * @return ExpressionResult status of the interpretation
 */
ExpressionResult Interpreter::interpretLine(std::string line, int lineNumber) {
	std::queue<Token*> tokens;
	ExpressionResult result = Lexer::tokenize(lineNumber, line, tokens, this->context);
	if (result.error()) return result;

	Lexer lexer(tokens, this->context);
	result = lexer.lex();
	if (result.error()) return result;

	Value::deleteValue(&this->lastValue);
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
		Value::deleteValue(&this->lastValue);
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
		Value::deleteValue(&last);
		Value::deleteValue(&this->memory.pop());

		return result;
	}

	Value::deleteValue(&this->lastValue);
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
				true
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
	for (size_t i = token->getParts().size() - 1; i > 0; i--) {
		str = token->getParts().at(i) + str;
		ExpressionResult result = this->memory.popVariableValue(value, this->context);
		if (result.error()) return result;
		str = value->getStringValue() + str;
		if (i == 1) range.merge(value->getRange());
		Value::deleteValue(&value);
	}
	str = token->getParts().at(0) + str;
	this->memory.push(new String(
		str,
		range,
		true
	));
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretOperator(const Token *operatorToken) {
	ExpressionResult sizeOk = this->memory.sizeExpected(
		2,
		"Not enough values for operator " + operatorToken->getStringValue(), 
		operatorToken->getRange(),
		this->context
	);
	if (sizeOk.error()) return sizeOk;
	Value *right, *left;
	ExpressionResult popOk = this->memory.popVariableValue(right, this->context);
	if (popOk.error()) return popOk;
	popOk = this->memory.popVariableValue(left, this->context);
	if (popOk.error()) {
		Value::deleteValue(&right);
		return popOk;
	}
	operatorResult result = left->applyOperator(right, operatorToken, this->context);
	Value::deleteValue(&right);
	Value::deleteValue(&left);
	if (std::get<0>(result).error()) {
		delete std::get<1>(result);
		return std::get<0>(result);
	}
	
	this->memory.push(std::get<1>(result));
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

ExpressionResult Interpreter::interpretAssignment(const Token *operatorToken) {
	ExpressionResult result = this->memory.sizeExpected(
		2,
		"Not enough values for operator " + operatorToken->getStringValue(), 
		operatorToken->getRange(),
		this->context
	);
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
	this->context->setValue(this->memory.top()->getStringValue(), left->copy(false), &hold);
	if (hold != nullptr) {
		if (this->lastValue == hold) 
			this->lastValue = nullptr;
		if (left != hold) 
			Value::deleteValue(&left);
	}
	
	return result;
}

ExpressionResult Interpreter::interpretFunctionCall(const Token *functionToken) {
	const RPNFunction *function;
	ExpressionResult result;
	if (builtins::builtinFunctions.contains(functionToken->getStringValue())) {
		function = &builtins::builtinFunctions.at(functionToken->getStringValue());
	} else {
		Value *value;
		result = this->context->getValue(functionToken, value);
		if (result.error()) return result;
		if (value->getType() != FUNCTION)
			return ExpressionResult(
				functionToken->getStringValue() + " is not a function",
				functionToken->getRange(),
				this->context
			);
		function = static_cast<Function*>(value)->getValue();
	}

	result = this->memory.sizeExpected(
		function->getArgumentsCount(),
		"Invalid number of arguments for function, got " + functionToken->getStringValue() +
		" but expected " + std::to_string(function->getArgumentsCount()) + " arguments",
		functionToken->getRange(),
		this->context
	);
	if (result.error()) return result;

	std::vector<Value*> arguments;
	for (size_t i = 0; i < function->getArgumentsCount(); i++) {
		Value *value;
		result = this->memory.popVariableValue(value, this->context);
		if (result.error()) return result;
		arguments.insert(arguments.begin(), value);
	}
	
	RPNFunctionResult callResult = function->call(arguments, functionToken->getRange(), this->context);
	for (Value *value : arguments) Value::deleteValue(&value);
	if (std::get<0>(callResult).error()) {
		Value::deleteValue(&std::get<1>(callResult));
		return std::get<0>(callResult);
	}
	this->memory.push(std::get<1>(callResult));
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretIf(Line &line, CodeBlock &block) {
	ExpressionResult result = this->interpretLine(line);
	if (result.error()) return result;
	Value *condition = this->lastValue->to(BOOL);
	if (static_cast<Bool*>(condition)->getValue()) {
		Value::deleteValue(&condition);
		return this->interpret(block.getBlocks());
	}
	Value::deleteValue(&condition);
	if (block.getNext() != nullptr)
		return this->interpret(block.getNext()->getBlocks());
	
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretWhile(Line &line, CodeBlock &block) {
	ExpressionResult result = this->interpretLine(line);
	if (result.error()) return result;
	Value *condition = this->lastValue->to(BOOL);
	while (static_cast<Bool*>(condition)->getValue()) {
		Value::deleteValue(&condition);
		block.reset();
		result = this->interpret(block.getBlocks());
		if (result.error()) return result;
		if (result.breakingLoop()) return ExpressionResult();
		result = this->interpretLine(line);
		if (result.error()) return result;
		condition = static_cast<Bool*>(this->lastValue->to(BOOL));
	}
	Value::deleteValue(&condition);
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
	std::vector<Int> forParams;
	Value *param;
	Value *intParam;
	for (int i = 0; i < 3; i++) {
		result = this->memory.popVariableValue(param, this->context);
		if (result.error()) return result;
		if (!param->isCastableTo(INT)) {
			return ExpressionResult(
				"Invalid type for for loop parameter, expected int but got " + Value::stringType(param->getType()),
				block.getRange(),
				this->context
			);
		}
		intParam = param->to(INT);
		intParam->interpreterValue = false;
		forParams.emplace(forParams.begin(), *static_cast<Int*>(intParam));
		delete intParam;
		Value::deleteValue(&param);
	}

	Value *variable = this->memory.pop();
	if (variable->getType() != VARIABLE) {
		TextRange range = variable->getRange();
		Value::deleteValue(&variable);
		return ExpressionResult(
			"Can't assign value to a non variable",
			range,
			this->context
		);
	}
	Int zero {0, TextRange(), false};
	CPPInterface step {&forParams.at(2)};
	if (step == &zero) {
		Value::deleteValue(&variable);
		return ExpressionResult(
			"Step can't be 0",
			forParams.at(2).getRange(),
			this->context
		);
	}

	CPPInterface i {&forParams.at(0)};
	while (!result.breakingLoop() && ((step > &zero && i < &forParams.at(1)) || (step < &zero && i > &forParams.at(1)))) {
		this->context->setValue(
			variable->getStringValue(),
			i.getValue()->copy(false)
		);
		result = this->interpret(block.getBlocks());
		if (result.error()) return result;
		i += step;
	}
	delete i.getValue();
	Value::deleteValue(&variable);
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
	if (result.error()) {
		// catch
		if (block.getNext()->getKeyword()->getStringValue() == "catch") {
			this->context->setValue(
				line.top()->getStringValue(),
				new String(result.getErrorMessage(), line.top()->getRange(), false)
			);
		
			result = this->interpret(block.getNext()->getBlocks());
			if (result.error()) return result;

			// finally
			if (block.getNext()->getNext() != nullptr)
				return this->interpret(block.getNext()->getNext()->getBlocks());
		}
	}
	// finally
	if (block.getNext()->getKeyword()->getStringValue() == "finally")
		return this->interpret(block.getNext()->getBlocks());
	return result;
}