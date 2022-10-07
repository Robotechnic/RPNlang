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

	if (this->lastValue != nullptr) {
		delete this->lastValue;
		this->lastValue = nullptr;
	}

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

	if (this->lastValue != nullptr) {
		delete this->lastValue;
		this->lastValue = nullptr;
	}

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

	this->lastValue = this->memory.pop();
	return ExpressionResult();
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
			if (!blocks.empty() && blocks.front()->getType() == CODE_BLOCK)
				result = this->interpretBlock(*l, *static_cast<CodeBlock*>(blocks.front()));
			else
				result = interpretLine(*l);
		} else {
			result = ExpressionResult(
				"Lexer didn't to its job corectly ;(",
				static_cast<CodeBlock*>(blocks.front())->getKeyword()->getRange(),
				this->context
			);
		}
		delete block;
	}

	if (result.error()) return result;
	return this->checkMemory();
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
				result = this->interpretFsString(static_cast<FStringToken*>(token));
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
	std::cout<<"Params line :"<<std::endl;
	line.display();
	std::cout<<"Code block :"<<std::endl;
	block.display();
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretFsString(FStringToken *token) {
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
	for (size_t i = 1; i < token->getParts().size(); i++) {
		Value *value = this->memory.pop();
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
	Value *right = this->memory.pop();
	Value *left = this->memory.pop();
	operatorResult result = left->applyOperator(right, operatorToken, this->context);
	delete left;
	delete right;
	if (std::get<0>(result).error()) 
		return std::get<0>(result);
	
	this->memory.push(std::get<1>(result));
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretFunctionCall(Token *functionToken) {
	return ExpressionResult(
		"Function call not implemented yet",
		functionToken->getRange(),
		this->context
	);
}