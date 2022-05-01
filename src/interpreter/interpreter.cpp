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
	return true;
}

/**
 * @brief return the last value remaining in the interpreter memory
 * 
 * @return Value the last value in the stack
 */
Value Interpreter::getLastValue() const {
	return this->lastValue;
}

/**
 * @brief take a string line, parse it and interpret the result
 * 
 * @param line the line to interpret
 * @return ExpressionResult status of the interpretation
 */
ExpressionResult Interpreter::interpret(std::string line) {
	std::queue<Token> tokens;
	std::string errorMessage;
	ExpressionResult result = Token::tokenize(0, line, tokens);
	if (result.error()) return result;

	result = this->interpret(tokens, 0);

	return result;
}

/**
 * @brief skip all separators in the token queue
 * 
 * @param tokens the token queue
 */
void Interpreter::skipSeparators(std::queue<Token> &tokens) {
	while (
		tokens.front().getType() == TOKEN_TYPE_END_OF_LINE || 
		tokens.front().getType() == TOKEN_TYPE_EXPRESSION_SEPARATOR
	) {
		tokens.pop();
	}
}

/**
 * @brief clear the interpreter memory
 */
void Interpreter::clearMemory() {
	while (!this->memory.empty()) {
		this->memory.pop();
	}
}

/**
 * @brief get the minimum and maximum text range of value list
 * 
 * @param values values that the text range will be calculated from
 * @return TextRange text range of the tokens list
 */
TextRange Interpreter::mergeRanges(const std::vector<Value> &values) {
	if (values.size() == 0) return TextRange();
	
	TextRange range = values.begin()->getRange();
	for (const Value &token : values) {
		range.merge(token.getRange());
	}
	
	return range;
}

/**
 * @brief check if the interpreter memory contain only one value
 * 
 * @param line the current line number in the file
 * @return ExpressionResult the result of the check
 */
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

/**
 * @brief try to extract values needed of a given math operator, apply it and push the result in the memory
 * 
 * @param mathOperator the token which represent the math operator
 * @return ExpressionResult result of the operation
 */
ExpressionResult Interpreter::applyOperator(const Token &mathOperator) {
	if (memory.size() < 2) {
		return ExpressionResult("Not enough arguments for operator " + mathOperator.getValue(), mathOperator.getRange());
	}
	ExpressionResult result;
	Value second = memory.top();
	memory.pop();
	return this->memory.top().applyOperator(second, mathOperator, this->variables);
}

/**
 * @brief affect a value to a given variable
 * 
 * @param affectToken the token which represent the affectation (=)
 * @return ExpressionResult result of the affectation
 */
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

/**
 * @brief check if the current token refer to a function
 * 
 * @param functionName token which represent the function call
 * @param builtin boolean to know if the function is a builtin function or an user defined one
 * @param argCount int to know how many arguments the function need
 * @return ExpressionResult result of the check
 */
ExpressionResult Interpreter::isFunction(const Token &functionName, bool &builtin, int &argCount) {
	const std::string name = functionName.getValue();
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

/**
 * @brief check if the memory contain enough values to apply a function and get values of variables
 * 
 * @param literalToken the token which represent the function call
 * @param argCount number of arguments needed
 * @param args A vector of Value to store the arguments
 * @return ExpressionResult status of the check
 */
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

/**
 * @brief call a function and push the result in the memory
 * 
 * @param functionName the token which represent the function call
 * @return ExpressionResult result of the call
 */
ExpressionResult Interpreter::callFunction(const Token &functionName) {
	// check if literal is a function name
	ExpressionResult result;
	std::string name = functionName.getValue();
	int argCount = 0;
	bool builtin = true;

	result = this->isFunction(functionName, builtin, argCount);
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

/**
 * @brief Extract text between each placeholder for values
 * 
 * @param fStringToken the token which represent the fstring
 * @param substrings the vector of string to store the substrings
 * @return ExpressionResult if each placeholder is valid
 */
ExpressionResult Interpreter::extractFStringSubstrings(const Token &fStringToken, std::vector<std::string> &substrings) {
	std::string fString = fStringToken.getValue();
	TextRange range = fStringToken.getRange();

	substrings.clear();
	substrings.push_back("");

	char c;
	int column = 0;
	while (fString.size() > 0) {
		c = fString.at(0);
		fString.erase(fString.begin());
		column++;
		if (c == '{') {
			if (fString.size() == 0 || fString[0] != '}') {
				return ExpressionResult(
					"Expected '}' after '{'", 
					TextRange(range.line, range.columnStart + column, 1)
				);
			}
			fString.erase(fString.begin());
			column++;
			substrings.push_back("");
		} else if (c == '}') {
			return ExpressionResult(
				"Expected '{' before '}'", 
				TextRange(range.line, range.columnStart + column, 1)
			);
		} else {
			substrings.at(substrings.size() - 1) += c;
		}
	}
	return ExpressionResult();
}

/**
 * @brief parse a given fstring and replace all {} by the values in the memory
 * 
 * @param fStringToken the token which represent the fstring
 * @return ExpressionResult result of the parsing
 */
ExpressionResult Interpreter::parseFString(const Token &fStringToken) {
	std::vector<std::string> substrings;

	ExpressionResult result = this->extractFStringSubstrings(fStringToken, substrings);
	if (result.error()) return result;

	int argCount = substrings.size() - 1;

	if (argCount > (int)memory.size()) {
		return ExpressionResult("Not enough arguments for fstring", fStringToken.getRange());
	}

	std::vector<Value> args(argCount);
	for (int i = argCount - 1; i >= 0; i--) {
		args.at(i) = memory.top();
		memory.pop();
	}

	std::stringstream formatedResult;
	for (size_t i = 0; i < substrings.size(); i++) {
		formatedResult << substrings.at(i);
		if (i < args.size())
			formatedResult << args.at(i).getStringValue();
	}

	memory.push(Value(formatedResult.str(), this->mergeRanges(args).merge(fStringToken.getRange())));

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
		case TOKEN_TYPE_FSTRING:
			return this->parseFString(tok);
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