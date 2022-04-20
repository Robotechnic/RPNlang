#include "interpreter/interpreter.hpp"

Interpreter::Interpreter() {}
Interpreter::Interpreter(std::map<std::string, Value> variables, std::map<std::string, RPNFunction> functions) {
	this->variables = variables;
	this->functions = functions;
}
Interpreter::Interpreter(std::string fileName) : fileName(fileName){}
Interpreter::~Interpreter() {
	if (this->file.is_open()) {
		this->file.close();
	}
	this->variables.clear();
	this->functions.clear();
}

bool Interpreter::interpretFile(std::string &errorMessage) {
	try {
		this->file.open(this->fileName);
		if (file.fail()) {
			errorMessage = "Failled to open file " + this->fileName + " (" + strerror(errno) + ")";
			return false;
		}
	} catch (const std::exception &e) {
		errorMessage = "File stream error :" + std::string(e.what());
		return false;
	}

	// std::string line;
	// std::getline(this->file, line);
	// bool error = true;
	// int lineNumber = 0;
	// std::vector<Token> tokens;
	// while (!error && !this->file.eof()) {
	// 	lineNumber ++;
	// 	error = Token::tokenize(lineNumber, line, tokens, errorMessage);
	// 	if (error)
	// 		continue;
	// 	error = this->interpret(tokens, errorMessage);
	// 	if (error)
	// 		continue;
	// 	std::getline(this->file, line);
	// }

	// return error;
	return true;
}

Value Interpreter::getLastValue() const {
	return this->lastValue;
}

ExpressionResult Interpreter::interpret(std::string line) {
	std::vector<Token> tokens;
	std::string errorMessage;
	ExpressionResult result = Token::tokenize(0, line, tokens);
	if (result.error()) return result;

	result = this->interpret(tokens, 0);

	return result;
}

void Interpreter::clearMemory() {
	while (!this->memory.empty()) {
		this->memory.pop();
	}
}

ExpressionResult Interpreter::checkMemory(int line) {
	if (memory.size() != 1) {
		if (memory.size() == 0) {
			return ExpressionResult("No result", TextRange(line, 0, 0));
		}
		unsigned int columnEnd = memory.top().getRange().columnEnd;
		return ExpressionResult("To much remaining arguments", TextRange(line, 0, columnEnd));
	}
	ExpressionResult result = memory.top().setVariable(this->variables);
	if (result.error()) return result;
	this->lastValue = memory.top();

	return ExpressionResult();
}

ExpressionResult Interpreter::applyOperator(Token mathOperator) {
	if (memory.size() < 2) {
		return ExpressionResult("Not enough arguments for operator " + mathOperator.getValue(), mathOperator.getRange());
	}
	ExpressionResult result;
	Value second = memory.top();
	memory.pop();
	return this->memory.top().applyOperator(second, mathOperator, this->variables);
}

ExpressionResult Interpreter::affectVariable(Token affectToken) {
	if (memory.size() < 2) {
		return ExpressionResult("Not enough arguments for affectation", affectToken.getRange());
	}
	Value second = memory.top();
	ExpressionResult result = second.setVariable(this->variables);
	if (result.error()) return result;
	memory.pop();

	if (memory.top().getType() != VARIABLE) {
		return ExpressionResult("Expression error", affectToken.getRange());
	}

	this->variables[memory.top().getStringValue()] = second;
	memory.pop();
	memory.push(second);
}

ExpressionResult Interpreter::createFunction(std::vector<Token> &tokens, Token affectToken, std::string body) {
	TextRange range = tokens[0].getRange();
	range.columnEnd = affectToken.getRange().columnEnd;
	if (tokens.size() < 1) {
		return ExpressionResult("No function name provided", tokens[0].getRange());
	}

	return ExpressionResult();
}

ExpressionResult Interpreter::interpret(std::vector<Token> tokens, int line) {
	this->clearMemory();
	ExpressionResult result;

	while (tokens.size() > 0) {
		Token tok = tokens[0];
		if (tok.isNumber() || tok.getType() == TOKEN_TYPE_STRING || tok.getType() == TOKEN_TYPE_LITERAL) {
			memory.push(Value(tok.getValue(), tok.getType(), tok.getLine(), tok.getColumn()));
		} else if (tok.getType() == TOKEN_TYPE_OPERATOR) {
			result = this->applyOperator(tok);
			if (result.error()) return result;
		} else if (tok.getType() == TOKEN_TYPE_AFFECT) {
			result = this->affectVariable(tok);
		} else {
			return ExpressionResult("Invalid token" + tok.getValue(), tok.getRange());
		}
		tokens.erase(tokens.begin());
	}
	
	return this->checkMemory(line);
}