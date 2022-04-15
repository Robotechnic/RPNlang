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

ExpressionResult Interpreter::interpret(std::string line) {
	std::vector<Token> tokens;
	std::string errorMessage;
	ExpressionResult result = Token::tokenize(0, line, tokens);
	if (result.error()) return result.setErrorredLine(line);

	result = this->interpret(tokens).setErrorredLine(line);

	return result;
}

ExpressionResult Interpreter::interpret(std::vector<Token> &tokens) {
	std::stack<Value> memory;
	std::cout<<tokens<<std::endl;

	for (const Token &tok : tokens) {
		if (tok.isNumber() || tok.getType() == TOKEN_TYPE_STRING || tok.getType() == TOKEN_TYPE_LITERAL) {
			memory.push(Value(tok.getValue(), tok.getType(), tok.getLine(), tok.getColumn()));
		} else if (tok.getType() == TOKEN_TYPE_OPERATOR) {
			if (memory.size() < 2) {
				return ExpressionResult("Not enough arguments for operator " + tok.getValue(), tok.getRange());
			}
			ExpressionResult result;
			Value second = memory.top();
			result = second.setVariable(this->variables);
			if (result.error()) return result;
			memory.pop();
			result = memory.top().setVariable(this->variables);
			if (result.error()) return result;
			switch (tok.getValue()[0]) {
				case '+':
					result = memory.top().add(second);
					break;
				case '-':
					result = memory.top().substract(second);
					break;
				case '*':
					result = memory.top().multiply(second);
					break;
				case '/':
					result = memory.top().divide(second);
					break;
			}
			if (result.error()) return result;
		} else if (tok.getType() == TOKEN_TYPE_AFFECT) {
			if (memory.size() < 2) {
				return ExpressionResult("Not enough arguments for affectation", tok.getRange());
			}
			Value second = memory.top();
			ExpressionResult result = second.setVariable(this->variables);
			if (result.error()) return result;
			memory.pop();

			if (memory.top().getType() != VARIABLE) {
				return ExpressionResult("Expression error", tok.getRange());
			}

			this->variables[memory.top().getValue()] = second;
			memory.pop();
			memory.push(second);
		} else {
			return ExpressionResult("Invalid token" + tok.getValue(), tok.getRange());
		}
	}
	
	if (memory.size() != 1) {
		if (memory.size() == 0) {
			return ExpressionResult("No result", TextRange(0, 0, 0));
		}
		return ExpressionResult("To much remaining arguments", TextRange(0, 0, 0));
	}
	ExpressionResult result = memory.top().setVariable(this->variables);
	if (result.error()) return result;
	std::cout<<memory.top().getValue()<<std::endl;

	return ExpressionResult();
}