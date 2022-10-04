#include "interpreter/interpreter.hpp"

Interpreter::Interpreter() :
	returnValue(nullptr),
	context(new Context("main", "<stdin>"))
{}

Interpreter::Interpreter(Context *ctx) : 
	returnValue(nullptr),
	context(ctx)
{}

Interpreter::~Interpreter() {
	delete returnValue;
	while (!memory.empty()) {
		delete memory.top();
		memory.pop();
	}
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
	std::queue<Token> tokens;
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
		tokens.push(Token(line, instruction.size(), TOKEN_TYPE_END_OF_LINE, "\n"));
	}
	file.close();
	
	if (error) return false;				

	Lexer lexer(tokens, this->context);
	result = lexer.lex();
	if (result.error()) {
		result.display();
		return false;
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
	std::queue<Token> tokens;
	ExpressionResult result = Lexer::tokenize(lineNumber, line, tokens, this->context);
	if (result.error()) return result;

	Lexer lexer(tokens, this->context);
	result = lexer.lex();
	if (result.error()) return result;

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
void Interpreter::clearQueue(std::queue<Token> &tokens) {
	while (!tokens.empty()) {
		tokens.pop();
	}
}

/**
 * @brief clear the interpreter memory
 */
void Interpreter::clearMemory(long unsigned int offset) {
	while (!this->memory.empty() && this->memory.size() > offset) {
		if (this->memory.top() != nullptr) delete this->memory.top();
		this->memory.pop();
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

ExpressionResult Interpreter::checkMemory() {
	if (this->memory.empty()) {
		this->lastValue = None::empty();
		return ExpressionResult();
	}

	if (this->memory.size() > 1) {
		Value *last = this->memory.top();
		this->memory.pop();
		clearMemory(1);
		ExpressionResult result(
			"To much remaining values in the memory",
			this->mergeRanges({last, this->memory.top()}),
			this->context
		);
		delete last;
		delete this->memory.top();
		this->memory.pop();
		return result;
	}

	this->lastValue = this->memory.top();
	this->memory.pop();
	return ExpressionResult();
}

ExpressionResult Interpreter::interpret(BlockQueue &blocks) {
	ExpressionResult result;
	
	while (!blocks.empty()) {
		BaseBlock *block = blocks.front();
		blocks.pop();
		if (block->getType() == LINE_BLOCK) {
			Line *l = static_cast<Line*>(block);
			if (!blocks.empty() && blocks.front()->getType() == CODE_BLOCK)
				result = this->interpretBlock(*l, *static_cast<CodeBlock*>(blocks.front()));
			else
				result = interpretLine(*l);
		} else {
			return ExpressionResult(
				"Lexer didn't to its job corectly ;(",
				static_cast<CodeBlock*>(blocks.front())->getKeyword().getRange(),
				this->context
			);
			throw std::runtime_error("Lexer didn't do its job correctly ;(");
		}
		if (result.error()) return result;
	}

	return this->checkMemory();
}

ExpressionResult Interpreter::interpretLine(Line &line) {
	auto tokens = line.getTokens();
	Token t;
	while (!tokens.empty()) {
		t = tokens.front();
		tokens.pop();
		switch (t.getType()) {
			default:
				return ExpressionResult(
					"Uexpected token",
					t.getRange(),
					this->context
				);
		}
	}
	return ExpressionResult();
}

ExpressionResult Interpreter::interpretBlock(Line &line, CodeBlock &block) {
	std::cout<<"Params line :"<<std::endl;
	line.display();
	std::cout<<"Code block :"<<std::endl;
	block.display();
	return ExpressionResult();
}