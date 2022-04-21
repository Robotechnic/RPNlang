#include "interpreter/interpreter.hpp"

Interpreter::Interpreter() {}
Interpreter::Interpreter(std::map<std::string, Value> variables, std::map<std::string, RPNFunction*> functions) {
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
		unsigned int columnEnd = memory.top().getRange().columnEnd - 1;
		return ExpressionResult("To much remaining arguments: " + std::to_string(memory.size()), TextRange(line, 0, columnEnd));
	}

	ExpressionResult result = memory.top().setVariable(this->variables);
	if (result.error()) return result;
	this->lastValue = memory.top();

	return ExpressionResult();
}

ExpressionResult Interpreter::applyOperator(const Token &mathOperator) {
	if (memory.size() < 2) {
		return ExpressionResult("Not enough arguments for operator " + mathOperator.getValue(), mathOperator.getRange());
	}
	ExpressionResult result;
	Value second = memory.top();
	memory.pop();
	return this->memory.top().applyOperator(second, mathOperator, this->variables);
}

ExpressionResult Interpreter::affectVariable(const Token &affectToken) {
	if (memory.size() < 2) {
		return ExpressionResult("Not enough arguments for affectation", affectToken.getRange());
	}
	Value second = memory.top();
	ExpressionResult result = second.setVariable(this->variables);
	if (result.error()) return result;
	memory.pop();

	if (memory.top().getType() != VARIABLE) {
		return ExpressionResult("Expected variable name but got (" + std::to_string(memory.top()) + ")", affectToken.getRange());
	}

	this->variables[memory.top().getStringValue()] = second;
	memory.pop();
	memory.push(second);

	return ExpressionResult();
}

ExpressionResult Interpreter::checkLiteral(const Token &literalToken) {
	// check if literal is a function name
	ExpressionResult result;
	int argCount = 0;
	std::string name = literalToken.getValue();
	bool builtin = true;

	if (this->functions.find(name) != this->functions.end()) {
		builtin = false;
		argCount = this->functions[name]->getArgumentsCount();
	} else if (BuiltinRPNFunction::builtinFunctions.find(name) != BuiltinRPNFunction::builtinFunctions.end()) {
		argCount = BuiltinRPNFunction::builtinFunctions.at(name).getArgumentsCount();
	} else {
		memory.push(Value(name, literalToken.getType(), literalToken.getLine(), literalToken.getColumn()));
		return ExpressionResult();
	}

	if (memory.size() < argCount) {
		return ExpressionResult("Not enough arguments for function " + name, literalToken.getRange());
	}

	RPNFunctionArgs args;
	Value arg;
	for (int i = 0; i < argCount; i++) {
		arg = memory.top();
		result = arg.setVariable(this->variables);
		if (result.error()) return result;
		args.insert(args.begin(), arg);
		memory.pop();
	}

	RPNFunctionResult functionResult;
	if (builtin) {
		functionResult = BuiltinRPNFunction::builtinFunctions.at(name).call(args, this->variables, this->functions);
	} else {
		functionResult = this->functions[name]->call(args, this->variables, this->functions);
	}

	result = std::get<0>(functionResult);
	if (result.error()) return result;
	
	this->memory.push(std::get<1>(functionResult));
	return ExpressionResult();
}

ExpressionResult Interpreter::parseKeyword(const Token &keywordToken, std::vector<Token> &tokens) {
	std::string keyword = keywordToken.getValue();

	if (keyword == "def") {
		return this->createFunction(keywordToken, tokens);
	}

	return ExpressionResult("Not implemented yet", keywordToken.getRange());
}

ExpressionResult Interpreter::createFunction(const Token &keywordToken, std::vector<Token> &tokens) {
	

	return ExpressionResult();
}

ExpressionResult Interpreter::interpret(std::vector<Token> tokens, int line) {
	this->clearMemory();
	ExpressionResult result;

	while (tokens.size() > 0) {
		Token tok = *tokens.begin();
		tokens.erase(tokens.begin());
		switch (tok.getType()) {
			case TOKEN_TYPE_FLOAT:
			case TOKEN_TYPE_INT:
			case TOKEN_TYPE_STRING:
			case TOKEN_TYPE_BOOL:
				this->memory.push(Value(tok.getValue(), tok.getType(), tok.getLine(), tok.getColumn()));
				break;
			case TOKEN_TYPE_OPERATOR:
				result = this->applyOperator(tok);
				break;
			case TOKEN_TYPE_LITERAL:
				result = this->checkLiteral(tok);
				break;
			case TOKEN_TYPE_AFFECT:
				result = this->affectVariable(tok);
				break;
			case TOKEN_TYPE_KEYWORD:
				result = this->parseKeyword(tok, tokens);
				break;
			default:
				result = ExpressionResult("Invalid token " + tok.getValue(), tok.getRange());
		}
		if (result.error()) return result;
	}
	
	return this->checkMemory(line);
}