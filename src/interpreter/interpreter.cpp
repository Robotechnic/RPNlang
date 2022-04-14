#include "interpreter/interpreter.hpp"

Interpreter::Interpreter() {}
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

	std::string line;
	std::getline(this->file, line);
	bool error = true;
	int lineNumber = 0;
	std::vector<Token> tokens;
	while (!error && !this->file.eof()) {
		lineNumber ++;
		error = Token::tokenize(lineNumber, line, tokens, errorMessage);
		if (error)
			continue;
		error = this->interpret(tokens, errorMessage);
		if (error)
			continue;
		std::getline(this->file, line);
	}

	return error;
}

bool Interpreter::interpret(std::string line, std::string &errorMessage) {
	std::vector<Token> result;
	if (!Token::tokenize(0, line, result, errorMessage)) return false;
	if (!this->interpret(result, errorMessage)) return false;

	return true;
}

bool Interpreter::interpret(std::vector<Token> &tokens, std::string &errorMessage) {
	std::stack<Value> memory;
	std::cout<<tokens<<std::endl;

	for (const Token &tok : tokens) {
		if (tok.isNumber() || tok.getType() == TOKEN_TYPE_LITERAL || tok.getType() == TOKEN_TYPE_STRING) {
			memory.push(Value(tok.getValue(), tok.getType()));
		} else if (tok.getType() == TOKEN_TYPE_OPERATOR) {
			if (memory.size() < 2) {
				errorMessage = "Expression error";
				return false;
			}
			Value second = memory.top();
			if (second.getType() == VARIABLE) {
				if (!second.setVariable(this->variables)) {
					errorMessage = "Variable " + second.getValue() + " not defined";
					return false;
				}
			}
			memory.pop();
			if (memory.top().getType() == VARIABLE) {
				if (!memory.top().setVariable(this->variables)) {
					errorMessage = "Variable " + memory.top().getValue() + " not defined";
					return false;
				}
			}
			bool result;
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
			if (!result) {
				errorMessage = "Arithmetic expression error";
				return false;
			}
		} else if (tok.getType() == TOKEN_TYPE_AFFECT) {
			if (memory.size() < 2) {
				errorMessage = "Expression error";
				return false;
			}
			Value affectVariable = memory.top();
			if (affectVariable.getType() == VARIABLE) {
				if (!affectVariable.setVariable(this->variables)) {
					errorMessage = "Variable " + affectVariable.getValue() + " not defined";
					return false;
				}
			}
			memory.pop();

			if (memory.top().getType() != VARIABLE) {
				errorMessage = "Expression error";
				return false;
			}

			this->variables[memory.top().getValue()] = affectVariable;
			memory.pop();
			memory.push(Value(affectVariable.getValue(), affectVariable.getType()));
		} else {
			errorMessage = "Invalid token";
			return false;
		}
	}
	
	if (memory.size() != 1) {
		errorMessage = "To much end values 1 expected but got "+std::to_string(memory.size());
		return false;
	}

	std::cout<<memory.top().getValue()<<std::endl;

	return true;
}