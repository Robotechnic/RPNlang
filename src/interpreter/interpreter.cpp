#include "interpreter/interpreter.hpp"

Interpreter::Interpreter() {}
Interpreter::Interpreter(std::map<std::string, Value> variables) {
	this->variables = variables;
}
Interpreter::Interpreter(std::string fileName) : fileName(fileName){}
Interpreter::~Interpreter() {
	if (this->file.is_open()) {
		this->file.close();
	}
	this->variables.clear();
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
	std::queue<Token> tokens;
	std::string errorMessage;
	ExpressionResult result = Token::tokenize(0, line, tokens);
	if (result.error()) return result;

	result = this->interpret(tokens, 0);

	return result;
}

void Interpreter::skipSeparators(std::queue<Token> &tokens) {
	while (
		tokens.front().getType() == TOKEN_TYPE_END_OF_LINE || 
		tokens.front().getType() == TOKEN_TYPE_EXPRESSION_SEPARATOR
	) {
		tokens.pop();
	}
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

ExpressionResult Interpreter::isFunction(const Token &functionName, std::string name, bool &builtin, int &argCount) {
	if (this->variables.find(name) != this->variables.end()) {
		builtin = false;
		if (this->variables[name].getType() != FUNCTION) {
			return ExpressionResult(
				name + " of value " + this->variables[name].getStringValue() + " with type " +
				this->variables[name].getStringValue() + " is not callable", 
				functionName.getRange()
			);
		}
		argCount = this->variables[name].getFunctionValue()->getArgumentsCount();
	} else if (BuiltinRPNFunction::builtinFunctions.find(name) != BuiltinRPNFunction::builtinFunctions.end()) {
		argCount = BuiltinRPNFunction::builtinFunctions.at(name).getArgumentsCount();
	} else {
		return ExpressionResult(
			"Function " + name + "not found",
			functionName.getRange()
		);
	}

	return ExpressionResult();
}

ExpressionResult Interpreter::checkArgs(const Token &literalToken, int argCount, RPNFunctionArgs &args) {
	if ((int)memory.size() < argCount) {
		return ExpressionResult("Not enough arguments for function " + literalToken.getValue(), literalToken.getRange());
	}

	ExpressionResult result;

	Value arg;
	for (int i = 0; i < argCount; i++) {
		arg = memory.top();
		result = arg.setVariable(this->variables);
		if (result.error()) return result;
		args.insert(args.begin(), arg);
		memory.pop();
	}

	return result;
}

ExpressionResult Interpreter::callFunction(const Token &functionName) {
	// check if literal is a function name
	ExpressionResult result;
	std::string name = functionName.getValue();
	int argCount = 0;
	bool builtin = true;

	result = this->isFunction(functionName, name, builtin, argCount);
	if (result.error()) return result;

	RPNFunctionArgs args;
	result = this->checkArgs(functionName, argCount, args);
	if (result.error()) return result;

	RPNFunctionResult functionResult = builtin ? 
		BuiltinRPNFunction::builtinFunctions.at(name).call(args, this->variables):
		this->variables.at(name).getFunctionValue()->call(args, this->variables);

	result = std::get<0>(functionResult);
	if (result.error()) return result;
	
	this->memory.push(std::get<1>(functionResult));
	return ExpressionResult();
}

ExpressionResult Interpreter::extractExpressionBody(std::queue<Token> &tokens, std::queue<Token> &expressionBody) {
	if (tokens.empty()) {
		return ExpressionResult("Unexpected end of line", TextRange(0, 0, 0));
	}
	while ( // expression body can only be one line for now
		!tokens.empty() &&
		tokens.front().getType() != TOKEN_TYPE_END_OF_LINE && 
		tokens.front().getType() != TOKEN_TYPE_EXPRESSION_SEPARATOR
	) {
		expressionBody.emplace(tokens.front());
		tokens.pop();
	}

	this->skipSeparators(tokens);

	if (expressionBody.size() == 0) {
		return ExpressionResult(
			"Missing expression after control sequence",
			tokens.front().getRange()
		);
	}

	return ExpressionResult();
}

ExpressionResult Interpreter::extractExpressionArguments(std::queue<Token> &tokens, std::queue<Token> &expressionArguments) {
	if (tokens.empty()) {
		return ExpressionResult("Unexpected end of line", TextRange(0, 0, 0));
	}
	while (
		!tokens.empty() &&
		tokens.front().getType() != TOKEN_TYPE_CONTROL_END &&
		tokens.front().getType() != TOKEN_TYPE_END_OF_LINE && 
		tokens.front().getType() != TOKEN_TYPE_EXPRESSION_SEPARATOR
	) {
		expressionArguments.emplace(tokens.front());
		tokens.pop();
	}

	if (tokens.front().getType() == TOKEN_TYPE_CONTROL_END) {
		tokens.pop();
		return ExpressionResult();
	}

	this->skipSeparators(tokens);
	
	return ExpressionResult("Missing : at end of expression line", tokens.front().getRange());
}

ExpressionResult Interpreter::parseKeyword(const Token &keywordToken, std::queue<Token> &tokens) {
	std::string keyword = keywordToken.getValue();

	return ExpressionResult("Not implemented yet", keywordToken.getRange());
}

ExpressionResult Interpreter::createFunction(const Token &keywordToken, std::queue<Token> &tokens) {
	return ExpressionResult();
}

ExpressionResult Interpreter::parseIf(const Token &keywordToken, std::queue<Token> &tokens) {
	return ExpressionResult();
}

ExpressionResult Interpreter::parseElse(std::queue<Token> &tokens) {
	return ExpressionResult();
}

ExpressionResult Interpreter::interpret(std::queue<Token> tokens, int line) {
	this->clearMemory();
	ExpressionResult result;

	while (tokens.size() > 0) {
		Token tok = tokens.front();
		tokens.pop();
		result = this->interpretToken(tok, tokens, line);
		if (result.error()) return result;
	}
	
	return this->checkMemory(line);
}

ExpressionResult Interpreter::interpretToken(const Token &tok, std::queue<Token> &tokens, int line) {
	switch (tok.getType()) {
		case TOKEN_TYPE_FLOAT:
		case TOKEN_TYPE_INT:
		case TOKEN_TYPE_STRING:
		case TOKEN_TYPE_BOOL:
		case TOKEN_TYPE_LITERAL:
			this->memory.push(Value(tok.getValue(), tok.getType(), tok.getLine(), tok.getColumn()));
			return ExpressionResult();
		case TOKEN_TYPE_OPERATOR:
		case TOKEN_TYPE_BOOLEAN_OPERATOR:
			return this->applyOperator(tok);
		case TOKEN_TYPE_FUNCTION_CALL:
			return this->callFunction(tok);
		case TOKEN_TYPE_AFFECT:
			return this->affectVariable(tok);
		case TOKEN_TYPE_KEYWORD:
			return this->parseKeyword(tok, tokens);
		case TOKEN_TYPE_EXPRESSION_SEPARATOR:
			return ExpressionResult();
		default:
			return ExpressionResult("Invalid token " + tok.getValue(), tok.getRange());
	}
}