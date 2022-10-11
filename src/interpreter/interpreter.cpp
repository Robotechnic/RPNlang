#include "interpreter/interpreter.hpp"

Interpreter::Interpreter() :
	returnValue(nullptr),
	lastValue(nullptr),
	context(new Context("main", "<stdin>"))
{}

Interpreter::Interpreter(Context *ctx) : 
	returnValue(nullptr),
	lastValue(nullptr),
	context(ctx)
{}

Interpreter::~Interpreter() {
	delete returnValue;
	delete lastValue;
	this->memory.clear();
}

bool Interpreter::openFile(std::ifstream &file, std::string fileName, std::string &error) {
	try {
		file.open(fileName);
		if (file.fail()) {
			error = "Failled to open file " + fileName + " : " + std::strerror(errno) + "";
			return false;
		}
	} catch (const std::exception &e) {
		error = "File stream error :" + std::string(e.what());
		return false;
	}

	return true;
}

bool Interpreter::interpretFile(std::string fileName, std::string &errorString) {
	std::ifstream file;
	if (!this->openFile(file, fileName, errorString)) {
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

	this->clearLastValue();

	result = this->interpret(lexer.getBlocks());
	if (result.error()) {
		result.display();
		return false;
	}

	return true;
}

/**
 * @brief take a string line, parse it and interpret the result
 * 
 * @param line the line to interpret
 * @return ExpressionResult status of the interpretation
 */
ExpressionResult Interpreter::interpretLine(std::string line, int lineNumber) {
	std::queue<Token*> tokens;
	ExpressionResult result = Lexer::tokenize(lineNumber, line, tokens, this->context);
	if (result.error()) return result;

	Lexer lexer(tokens, this->context);
	result = lexer.lex();
	if (result.error()) return result;

	this->clearLastValue();
	return this->interpret(lexer.getBlocks());
}

/**
 * @brief return the last value remaining in the interpreter memory
 * 
 * @return Value the last value in the stack
 */
Value *Interpreter::getLastValue() const {
	return this->lastValue;
}

Value *Interpreter::getReturnValue() const {
	return this->returnValue;
}

/**
 * @brief clear a token queue
 * 
 * @param tokens the queue to clear
 */
void Interpreter::clearQueue(std::queue<Token*> &tokens) {
	while (!tokens.empty()) {
		delete tokens.front();
		tokens.pop();
	}
}

/**
 * @brief to avoid memory leak, clear the last value before resetting it at the end of a line interpretation
 * 
 */
void Interpreter::clearLastValue() {
	if (this->lastValue != nullptr) {
		delete this->lastValue;
		this->lastValue = nullptr;
	}
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
		this->lastValue = None::empty();
		return ExpressionResult();
	}

	if (this->memory.size() > 1) {
		Value *last = this->memory.pop();
		this->memory.clear(1);
		ExpressionResult result(
			"To much remaining values in the memory",
			last->getRange().merge(this->memory.top()->getRange()),
			this->context
		);
		delete last;
		delete this->memory.pop();
		return result;
	}

	this->clearLastValue();
	return this->memory.popVariableValue(this->lastValue, this->context);
}

/**
 * @brief interpret a list of blocks
 * 
 * @param blocks the list of blocks to interpret
 * @return ExpressionResult status of the interpretation
 */
ExpressionResult Interpreter::interpret(BlockQueue &blocks) {
	ExpressionResult result;
	
	while (!blocks.empty() && !result.error()) {
		BaseBlock *block = blocks.front();
		blocks.pop();
		if (block->getType() == LINE_BLOCK) {
			Line *l = static_cast<Line*>(block);
			if (!blocks.empty() && blocks.front()->getType() == CODE_BLOCK) {
				result = this->interpretBlock(*l, *static_cast<CodeBlock*>(blocks.front()));
				delete blocks.front();
				blocks.pop();
			} else
				result = interpretLine(*l);
		} else {
			result = ExpressionResult(
				"Lexer didn't to its job corectly ;(",
				static_cast<CodeBlock*>(block)->getKeyword()->getRange(),
				this->context
			);
		}
		delete block;
	}

	return result;
}

/**
 * @brief interpret a line of tokens
 * 
 * @param line the line to interpret
 * @return ExpressionResult status of the interpretation
 */
ExpressionResult Interpreter::interpretLine(Line &line) {
	Token *token;
	ExpressionResult result;
	while (!line.empty() && !result.error()) {
		token = line.pop();
		switch (token->getType()) {
			case TOKEN_TYPE_VALUE:
				this->memory.push(static_cast<ValueToken*>(token)->getValue());
				break;
			case TOKEN_TYPE_OPERATOR:
			case TOKEN_TYPE_BOOLEAN_OPERATOR:
				result = this->interpretOperator(token);
				break;
			case TOKEN_TYPE_FUNCTION_CALL:
				result = this->interpretFunctionCall(token);
				break;
			case TOKEN_TYPE_FSTRING:
				result = this->interpretFString(static_cast<FStringToken*>(token));
				break;
			case TOKEN_TYPE_ASSIGNMENT:
				result = this->interpretAssignment(token);
				break;
			default:
				result = ExpressionResult(
					"Unknow token (TODO)",
					token->getRange(),
					this->context
				);
		}
		delete token;
	}
	return this->checkMemory();
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
	} else if (type == "fun") {
		return this->interpretFunction(line, block);
	} else {
		return ExpressionResult(
			"Unknow block type",
			block.getKeyword()->getRange(),
			this->context
		);
	}
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretFString(FStringToken *token) {
	ExpressionResult sizeOk = this->memory.sizeExpected(
		token->getParts().size() - 1,
		"missing fString parameters expected " + std::to_string(token->getParts().size() - 1) + 
								   " but got " + std::to_string(this->memory.size()),
		token->getRange(),
		this->context
	);
	TextRange range = token->getRange();
	if (sizeOk.error()) return sizeOk;
	std::string str = token->getParts().at(0);
	Value *value;
	for (size_t i = 1; i < token->getParts().size(); i++) {
		ExpressionResult result = this->memory.popVariableValue(value, this->context);
		if (result.error()) return result;
		str += value->getStringValue();
		if (i == 1) range.merge(value->getRange());
		delete value;
		str += token->getParts().at(i);
	}
	this->memory.push(new String(
		str,
		range
	));
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretOperator(Token *operatorToken) {
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
		delete right;
		return popOk;
	}
	operatorResult result = left->applyOperator(right, operatorToken, this->context);
	delete left;
	delete right;
	if (std::get<0>(result).error()) 
		return std::get<0>(result);
	
	this->memory.push(std::get<1>(result));
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretAssignment(Token *operatorToken) {
	ExpressionResult sizeOk = this->memory.sizeExpected(
		2,
		"Not enough values for operator " + operatorToken->getStringValue(), 
		operatorToken->getRange(),
		this->context
	);
	if (sizeOk.error()) return sizeOk;
	Value *left;
	ExpressionResult result = this->memory.popVariableValue(left, this->context);
	if (result.error()) return result;
	if (this->memory.top()->getType() != VARIABLE) {
		ExpressionResult result(
			"Can't assign value to a non variable",
			this->memory.top()->getRange(),
			this->context
		);
		delete left;
		return result;
	}
	this->context->setValue(this->memory.top()->getStringValue(), left);
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretFunctionCall(Token *functionToken) {
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
	RPNFunctionResult callResult = function->call(arguments, this->context);
	for (Value *value : arguments) delete value;
	if (std::get<0>(callResult).error()) {
		return std::get<0>(callResult);
	}
	this->memory.push(std::get<1>(callResult));
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretIf(Line &line, CodeBlock &block) {
	ExpressionResult result = this->interpretLine(line);
	if (result.error()) return result;
	Value *val = this->lastValue;
	Bool *condition = static_cast<Bool*>(val->to(BOOL));
	delete val;
	Line l;
	if (condition->getValue()) {
		delete condition;
		return this->interpret(block.getBlocks());
	}
	delete condition;
	if (block.getNext() != nullptr)
		return this->interpret(block.getNext()->getBlocks());
	
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretWhile(Line &line, CodeBlock &block) {
	ExpressionResult result = this->interpretLine(line);
	if (result.error()) return result;
	Value *val = this->lastValue;
	Bool *condition = static_cast<Bool*>(val->to(BOOL));
	delete val;
	Line l;
	while (condition->getValue()) {
		delete condition;
		result = this->interpret(block.getBlocks());
		if (result.error()) return result;
		result = this->interpretLine(line);
		if (result.error()) return result;
		val = this->memory.pop();
		condition = static_cast<Bool*>(val->to(BOOL));
		delete val;
	}
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretFor(Line &line, CodeBlock &block) {
	return ExpressionResult("TODO: implement for loop", line.lastRange(), this->context);
}

ExpressionResult Interpreter::interpretFunction(Line &line, CodeBlock &block) {
	return ExpressionResult("TODO: implement function", line.lastRange(), this->context);
}