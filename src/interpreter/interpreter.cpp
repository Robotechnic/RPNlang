#include "interpreter/interpreter.hpp"

Interpreter::Interpreter() : context("main") {}

Interpreter::Interpreter(Context ctx) : context(ctx) {}

Interpreter::~Interpreter() {
	this->context.clear();
}

bool Interpreter::openFile(std::ifstream &file, std::string fileName) {
	try {
		file.open(fileName);
		if (file.fail()) {
			std::cout<<"Failled to open file " << fileName << " (" << std::strerror(errno) << ")"<<std::endl;
			return false;
		}
	} catch (const std::exception &e) {
		std::cout << "File stream error :" << std::string(e.what()) << std::endl;
		return false;
	}

	return true;
}

bool Interpreter::interpretFile(std::string fileName) {
	std::ifstream file;
	if (!this->openFile(file, fileName)) {
		return false;
	}

	this->context = Context(fileName, CONTEXT_TYPE_FILE);
	bool errored = false;
	std::queue<Token> tokens;
	std::string instruction;
	int line = 0;
	ExpressionResult result;
	while (!errored && std::getline(file, instruction)) {
		line++;
		result = Token::tokenize(line, instruction, tokens, this->context);
		if (result.error()) {
			result.display(fileName);
			errored = true;
		}
		tokens.push(Token(line, instruction.size(), TOKEN_TYPE_END_OF_LINE, "\n"));
	}
	file.close();

	result = this->interpret(tokens);
	if (result.error()) {
		result.display(fileName);
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
ExpressionResult Interpreter::interpret(std::string line, int lineNumber) {
	std::queue<Token> tokens;
	std::string errorMessage;
	ExpressionResult result = Token::tokenize(lineNumber, line, tokens, this->context);
	if (result.error()) return result;

	result = this->interpret(tokens);

	return result;
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
 * @return ExpressionResult the result of the check
 */
ExpressionResult Interpreter::checkMemory() {
	if (memory.size() > 1) {
		TextRange lastTokenRange = memory.top().getRange();
		lastTokenRange.columnStart = 0;
		return ExpressionResult(
			"To much remaining elements: " + std::to_string(memory.size()), 
			lastTokenRange,
			this->context
		);
	}
	if (memory.size() == 1) {
		ExpressionResult result = memory.top().getVariableValue(this->context);
		if (result.error()) return result;
		this->lastValue = memory.top();
	} else {
		this->lastValue = Value();
	}

	this->clearMemory();

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
		return ExpressionResult(
			"Not enough arguments for operator " + mathOperator.getValue(), 
			mathOperator.getRange(),
			this->context
		);
	}
	ExpressionResult result;
	Value second = memory.top();
	memory.pop();
	return this->memory.top().applyOperator(second, mathOperator, this->context);
}

/**
 * @brief affect a value to a given variable
 * 
 * @param affectToken the token which represent the affectation (=)
 * @return ExpressionResult result of the affectation
 */
ExpressionResult Interpreter::affectVariable(const Token &affectToken) {
	if (memory.size() < 2) {
		return ExpressionResult(
			"Not enough arguments for affectation", 
			affectToken.getRange(),
			this->context
		);
	}

	Value second = memory.top();
	ExpressionResult result = second.getVariableValue(this->context);
	if (result.error()) return result;
	memory.pop();

	if (memory.top().getType() != VARIABLE) {
		return ExpressionResult(
			"Expected variable name but got (" + std::to_string(memory.top()) + ")", 
			affectToken.getRange(),
			this->context
		);
	}

	this->context.setValue(this->memory.top(), second);
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
	Value val;
	ExpressionResult result = this->context.getValue(functionName, val);
	if (!result.error()) {
		builtin = false;
		if (val.getType() != FUNCTION) {
			return ExpressionResult(
				name + " of value " + val.getStringValue() + " with type " +
				val.getStringValue() + " is not callable", 
				functionName.getRange(),
				this->context
			);
		}
		argCount = val.getFunctionValue()->getArgumentsCount();
	} else if (BuiltinRPNFunction::builtinFunctions.find(name) != BuiltinRPNFunction::builtinFunctions.end()) {
		argCount = BuiltinRPNFunction::builtinFunctions.at(name).getArgumentsCount();
	} else {
		return ExpressionResult(
			"Function '" + name + "' not found",
			functionName.getRange(),
			this->context
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
		return ExpressionResult(
			"Not enough arguments for function '" + literalToken.getValue() + "'",
			literalToken.getRange(),
			this->context
		);
	}

	ExpressionResult result;

	Value arg;
	for (int i = 0; i < argCount; i++) {
		arg = memory.top();
		result = arg.getVariableValue(this->context);
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
		BuiltinRPNFunction::builtinFunctions.at(name).call(args, this->context) :
		this->context.getValue(functionName).getFunctionValue()->call(args, this->context);

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
					TextRange(range.line, range.columnStart + column, 1),
					this->context
				);
			}
			fString.erase(fString.begin());
			column++;
			substrings.push_back("");
		} else if (c == '}') {
			return ExpressionResult(
				"Expected '{' before '}'", 
				TextRange(range.line, range.columnStart + column, 1),
				this->context
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
		return ExpressionResult(
			"Not enough arguments for fstring", 
			fStringToken.getRange(),
			this->context
		);
	}

	std::vector<Value> args(argCount);
	for (int i = argCount - 1; i >= 0; i--) {
		args.at(i) = memory.top();
		memory.pop();
	}

	std::stringstream formatedResult;
	for (size_t i = 0; i < substrings.size(); i++) {
		formatedResult << escapeCharacters(substrings.at(i));
		if (i < args.size()) {
			result = args.at(i).getVariableValue(this->context);
			if (result.error()) return result;
			formatedResult << args.at(i).getStringValue();
		}
	}

	memory.push(Value(formatedResult.str(), this->mergeRanges(args).merge(fStringToken.getRange())));

	return ExpressionResult();
}

/**
 * @brief extract body of an expression and stop to the expression end at the right level
 * 
 * @param keywordToken the token which represent the expression keyword
 * @param tokens tokens in the expression or file
 * @param expressionBody the body inside the expression
 * @param expressionEnd the closing token of the expression
 * @return ExpressionResult 
 */
ExpressionResult Interpreter::extractExpressionBody(const Token &keywordToken, std::queue<Token> &tokens, std::queue<Token> &expressionBody, Token &expressionEnd) {
	if (tokens.empty()) {
		return ExpressionResult(
			"Unexpected end of line after token '" + keywordToken.getValue() + "'", 
			keywordToken.getRange(),
			this->context
		);
	}
	
	// continue over lines until found the corresponding end at the correct level
	int level = 1;
	Token token;
	while (!tokens.empty() && level > 0) {
		token = tokens.front();
		tokens.pop();
		if (token.getType() == TokenType::TOKEN_TYPE_KEYWORD) {
			if (std::find(blockOpeners, blockOpeners + BLOCK_OPENERS, token.getValue()) != blockOpeners + BLOCK_OPENERS) {
				level++;
			} else if (std::find(blockClosers, blockClosers + BLOCK_CLOSERS, token.getValue()) != blockClosers + BLOCK_CLOSERS) {
				level--;
			}
		}
		if (level > 0) {
			expressionBody.emplace(token);
		} else {
			expressionEnd = token;
		}
	}

	if (level > 0) {
		expressionEnd = token;
	}

	if (expressionBody.empty()) {
		return ExpressionResult(
			"Expected expression body after token '" + keywordToken.getValue() + "'", 
			keywordToken.getRange(),
			this->context
		);
	}

	return ExpressionResult();
}

/**
 * @brief extract body of an expression and check if the closing token is correct
 * 
 * @param keywordToken the token which represent the expression keyword
 * @param tokens the tokens to parse
 * @param expressionBody the body of the expression
 * @param expressionEnd the expected content of the closing token
 * @return ExpressionResult if the closing token is correct
 */
ExpressionResult Interpreter::extractExpressionBody(const Token &keywordToken, std::queue<Token> &tokens, std::queue<Token> &expressionBody, std::string expressionEnd) {
	Token end;
	ExpressionResult result = this->extractExpressionBody(keywordToken, tokens, expressionBody, end);
	if (result.error()) return result;
	
	if (end.getValue() != expressionEnd)  {
		return ExpressionResult(
			"Expected '" + expressionEnd + "' keywords after '" + keywordToken.getValue() + "' statement", 
			end.getRange(),
			this->context
		);
	}

	return ExpressionResult();
}

/**
 * @brief Extract all tokens from the tokens queue until the encountered token match the given type and content it's ignore block level
 * 
 * @param keywordToken the token to extract from
 * @param tokens the tokens to parse
 * @param expressionBody the tokens until the provided end
 * @param tokenType the type of the token to extract
 * @param tokenValue the value of the token to extract
 * @return ExpressionResult if the closing token is found
 */
ExpressionResult Interpreter::getUntilToken(const Token &keywordToken, std::queue<Token> &tokens, std::queue<Token> &expressionBody, TokenType tokenType, std::string tokenValue) {
	if (tokens.empty()) {
		return ExpressionResult(
			"Unexpected end of line after token '" + keywordToken.getValue() + "'", 
			keywordToken.getRange(),
			this->context
		);
	}

	Token token;
	while (!tokens.empty()) {
		token = tokens.front();
		tokens.pop();
		if (token.getType() == tokenType && token.getValue() == tokenValue) {
			return ExpressionResult();
		}
		expressionBody.emplace(token);
	}

	return ExpressionResult(
		"Expected '" + tokenValue + "' after '" + keywordToken.getValue() + "' statement", 
		token.getRange(),
		this->context
	);
}

/**
 * @brief pasre a givent token, his body and its arguments and run the corresponding instruction
 * 
 * @param keywordToken the token which represent the keyword
 * @param tokens the tokens after the keyword in the expression
 * @param previous the tokens before the keyword in the expression
 * @return ExpressionResult if the parsing is successfull
 */
ExpressionResult Interpreter::parseKeyword(const Token &keywordToken, std::queue<Token> &tokens, const std::queue<Token> &previous) {
	std::string keyword = keywordToken.getValue();
	if (keyword == "fi") {
		return ExpressionResult(
			"Expected 'if' statement before 'fi' keyword", 
			keywordToken.getRange(),
			this->context
		);
	}

	if (keyword == "rof") {
		return ExpressionResult(
			"Expected 'for' statement before 'rof' keyword", 
			keywordToken.getRange(),
			this->context
		);
	}

	if (keyword == "elihw") {
		return ExpressionResult(
			"Expected 'while' statement before 'elihw' keyword", 
			keywordToken.getRange(),
			this->context
		);
	}

	if (keyword == "nuf") {
		return ExpressionResult(
			"Expected 'fun' statement before 'nuf' keyword", 
			keywordToken.getRange(),
			this->context
		);
	}

	if (keyword == "else") {
		return ExpressionResult(
			"Expected 'if' statement before 'else' keyword", 
			keywordToken.getRange(),
			this->context
		);
	}

	if (keyword == "if") {
		return this->parseIf(keywordToken, tokens);
	}

	if (keyword == "for") {
		return this->parseFor(keywordToken, tokens);
	}

	if (keyword == "while") {
		return this->parseWhile(keywordToken, tokens, previous);
	}

	if (keyword == "fun") {
		return this->createFunction(keywordToken, tokens);
	}

	return ExpressionResult(
		"Keyword " + keyword + " is not implemented yet", 
		keywordToken.getRange(),
		this->context
	);
}

/**
 * @brief parse an if expression in the tokens queue and run it if the condition is true
 * 
 * @param keywordToken the token which represent the if keyword
 * @param tokens the list of tokens to parse
 * @return ExpressionResult if there is and error in the expression
 */
ExpressionResult Interpreter::parseIf(const Token &keywordToken, std::queue<Token> &tokens) {
	if (this->memory.size() < 1) {
		return ExpressionResult(
			"Expected boolean expression before 'if'", 
			keywordToken.getRange(),
			this->context
		);
	}

	Value condition = this->memory.top();
	this->memory.pop();
	ExpressionResult result = condition.getVariableValue(this->context);
	if (result.error()) return result;
	
	//extract if body
	std::queue<Token> ifBody;
	Token ifEnd;
	result = this->extractExpressionBody(keywordToken, tokens, ifBody, ifEnd);
	if (result.error()) return result;

	if (ifEnd.getValue() != "fi" && ifEnd.getValue() != "else")  {
		return ExpressionResult(
			"Expected 'fi' or 'else' keywords after 'if' statement", 
			ifEnd.getRange(),
			this->context
		);
	}

	// run if body if condition is true
	bool conditionValue = condition.getBoolValue();
	if (conditionValue) {
		result = this->interpret(ifBody);
		if (result.error()) return result;
		this->memory.push(this->lastValue);
	}

	if (ifEnd.getValue() == "fi") {
		return ExpressionResult();
	}

	return this->parseElse(ifEnd, tokens, conditionValue);
}

/**
 * @brief parse else token and run the body if the previous if was false
 * 
 * @param keywordToken the token which represent the else keyword
 * @param tokens the list of tokens to parse
 * @param skipElse if true the else body will not be run
 * @return ExpressionResult 
 */
ExpressionResult Interpreter::parseElse(const Token &keywordToken, std::queue<Token> &tokens, bool skipElse) {
	// extract else body
	std::queue<Token> elseBody;
	ExpressionResult result = this->extractExpressionBody(keywordToken, tokens, elseBody, "fi");
	if (result.error()) return result;

	if (skipElse) {
		return ExpressionResult();
	}

	result = this->interpret(elseBody);
	if (result.error()) return result;
	this->memory.push(this->lastValue);
	return ExpressionResult();
}

/**
 * @brief extract and parse values in memory for the for loop
 * 
 * @param keywordToken the token which represent the for keyword
 * @param incrementName the name of the variable to increment
 * @param range the range of the for loop
 * @return ExpressionResult 
 */
ExpressionResult Interpreter::parseForParameters(const Token &keywordToken, std::string &incrementName, Value range[3] ) {
	if (this->memory.size() < 4) {
		return ExpressionResult(
			"Exprected start, end and step values before 'for' statement",
			keywordToken.getRange(),
			this->context
		);
	}

	ExpressionResult result;
	for (int i = 0; i < 3; i++) {
		range[i] = this->memory.top();
		this->memory.pop();
		result = range[i].getVariableValue(this->context);
		if (result.error()) return result;
		if (!range[i].isNumber()) {
			return ExpressionResult(
				"Expected number value for 'for' statement",
				range[i].getRange(),
				this->context
			);
		}
	}

	Value increment = this->memory.top();
	this->memory.pop();
	if (increment.getType() != VARIABLE) {
		return ExpressionResult(
			"Expected variable name to store incement",
			increment.getRange(),
			this->context
		);
	}

	incrementName = increment.getStringValue();

	if (range[2] > range[1]) {
		range[0] = -range[0];
		std::swap(range[2], range[1]);
	}

	return ExpressionResult();
}

/**
 * @brief extract for body and run it as a normal for loop does
 * 
 * @param keywordToken the token which represent the for keyword
 * @param tokens the list of tokens to parse
 * @return ExpressionResult if there is an error in the expression
 */
ExpressionResult Interpreter::parseFor(const Token &keywordToken, std::queue<Token> &tokens) {
	std::string incrementName;
	Value range[3];
	ExpressionResult result = this->parseForParameters(keywordToken, incrementName, range);
	if (result.error()) return result;

	// extract for body
	std::queue<Token> forBody;
	result = this->extractExpressionBody(keywordToken, tokens, forBody, "rof");
	if (result.error()) return result;

	// run for body for each value in range
	for (Value value = range[2]; value < range[1]; value += range[0]) {
		this->context.setValue(incrementName, value);
		result = this->interpret(forBody);
		if (result.error()) return result;
	}

	return ExpressionResult();
}

/**
 * @brief parse and run a while loop
 * 
 * @param keywordToken the token which represent the while keyword
 * @param tokens the list of tokens to parse
 * @param previous the previous tokens because in case of while the condition is evaluated each loop iteration
 * @return ExpressionResult if the loop is valid
 */
ExpressionResult Interpreter::parseWhile(const Token &keywordToken, std::queue<Token> &tokens, const std::queue<Token> &previous) {
	if (this->memory.size() < 1) {
		return ExpressionResult(
			"Expected boolean expression before 'while'", 
			keywordToken.getRange(),
			this->context
		);
	}

	Value condition = this->memory.top();
	this->memory.pop();
	ExpressionResult result = condition.getVariableValue(this->context);
	if (result.error()) return result;

	// extract while body
	std::queue<Token> whileBody;
	result = this->extractExpressionBody(keywordToken, tokens, whileBody, "elihw");
	if (result.error()) return result;

	bool continueLoop = condition.getBoolValue();
	while (continueLoop) {
		result = this->interpret(whileBody);
		if (result.error()) return result;

		this->interpret(previous);
		continueLoop = this->lastValue.getBoolValue();
	}

	return ExpressionResult();
}

/**
 * @brief extract and parse values for function arguments
 * 
 * @param keywordToken the token which represent the fun keyword
 * @param tokens the tokens to parse
 * @param name the string to store the function name in
 * @param argNames the vector to store the function arguments names in
 * @param types the vector to store the function arguments types in
 * @return ExpressionResult if there is an error in the arguments list
 */
ExpressionResult Interpreter::extractFunctionArgs(const Token &keywordToken, std::queue<Token> &tokens, std::string &name, std::vector<std::string> &argsNames, std::vector<ValueType> &types, ValueType &returnType) {
	std::queue<Token> args;
	ExpressionResult result = this->getUntilToken(keywordToken, tokens, args, TOKEN_TYPE_ARROW, "->");
	if (result.error()) return result;

	name = "";
	if (args.size() % 2 != 0) {
		Token functionName = args.front();
		args.pop();
		if (functionName.getType() != TOKEN_TYPE_LITERAL) {
			return ExpressionResult(
				"Expected function name",
				functionName.getRange(),
				this->context
			);
		}
		name = functionName.getValue();
	}

	while (!args.empty()) {
		if (argsNames.size() < types.size()) {
			if (args.front().getType() != TOKEN_TYPE_LITERAL) {
				return ExpressionResult(
					"Expected argument name",
					args.front().getRange(),
					this->context
				);
			}
			argsNames.push_back(args.front().getValue());
			args.pop();
		} else {
			if (args.front().getType() != TOKEN_TYPE_VALUE_TYPE) {
				return ExpressionResult(
					"Expected argument type",
					args.front().getRange(),
					this->context
				);
			}
			types.push_back(Value::valueType(args.front().getValue()));
			args.pop();
		}
	}

	result = this->getUntilToken(keywordToken, tokens, args, TOKEN_TYPE_COLON, ":");
	if (result.error()) return result;

	if (args.size() != 1) {
		if (args.size() == 0) {
			return ExpressionResult(
				"Missing return type for function '" + name + "'",
				keywordToken.getRange(),
				this->context
			);
		}
		return ExpressionResult(
			"Expected single token for return type but got " + std::to_string(args.size()) + " tokens",
			args.front().getRange(),
			this->context
		);
	}

	if (args.front().getType() != TOKEN_TYPE_VALUE_TYPE) {
		return ExpressionResult(
			"Expected return type",
			args.front().getRange(),
			this->context
		);
	}

	returnType = Value::valueType(args.front().getValue());

	return ExpressionResult();
}

/**
 * @brief extract arguments and body of a function
 * 
 * @param keywordToken the token which represent the fun keyword
 * @param tokens the list of tokens to parse
 * @return ExpressionResult if the function is correct or not
 */
ExpressionResult Interpreter::createFunction(const Token &keywordToken, std::queue<Token> &tokens) {
	// extract function name, args and return type
	std::string name;
	std::vector<std::string> argsNames;
	std::vector<ValueType> types;
	ValueType returnType;

	ExpressionResult result = this->extractFunctionArgs(keywordToken, tokens, name, argsNames, types, returnType);
	if (result.error()) return result;

	// extract body
	std::queue<Token> body;
	result = this->extractExpressionBody(keywordToken, tokens, body, "nuf");
	if (result.error()) return result;


	// build the function
	Value definedFunction = Value(
		new UserRPNFunction(
			name,
			argsNames,
			types,
			returnType,
			body
		),
		keywordToken.getRange().merge(body.back().getRange())
	);

	this->memory.push(definedFunction);
	
	if (name.size() == 0) return ExpressionResult();
	this->context.setValue(
		name,
		definedFunction
	);

	return ExpressionResult();
}

/**
 * @brief check if the token after : is a function name and if it is, call the function
 * 
 * @param keywordToken the token which represent the ':' keyword
 * @param tokens the list of tokens to parse
 * @return ExpressionResult if the function is correct and called wihout errors
 */
ExpressionResult Interpreter::parseFunctionCall(const Token &keywordToken, std::queue<Token> &tokens) {
	Token functionName = tokens.front();
	tokens.pop();
	if (functionName.getType() != TOKEN_TYPE_LITERAL) {
		return ExpressionResult(
			"Expected function name after ':' keyword",
			functionName.getRange(),
			this->context
		);
	}

	return this->callFunction(functionName);
}

/**
 * @brief take a list of tokens and interpret them
 * 
 * @param tokens list of tokens to interpret
 * @return ExpressionResult if there is an error in the expression
 */
ExpressionResult Interpreter::interpret(std::queue<Token> tokens) {
	this->clearMemory();
	ExpressionResult result;

	std::queue<Token> previous; // save previous tokens to check for while loops and for errors
	while (tokens.size() > 0) {
		Token tok = tokens.front();
		tokens.pop();
		result = this->interpretToken(tok, tokens, previous);
		if (result.error()) return result;
		previous.emplace(tok);
	}
	
	this->clearQueue(previous);
	return this->checkMemory();
}

/**
 * @brief interpret a single token
 * 
 * @param tok the token to interpret
 * @param tokens all the tokens after the current token in case where the token is a keyword
 * @param previous the previous tokens in case where the token is a keyword
 * @return ExpressionResult if there is an error in the interpretation
 */
ExpressionResult Interpreter::interpretToken(const Token &tok, std::queue<Token> &tokens, std::queue<Token> &previous) {
	switch (tok.getType()) {
		case TOKEN_TYPE_FLOAT:
		case TOKEN_TYPE_INT:
		case TOKEN_TYPE_BOOL:
		case TOKEN_TYPE_LITERAL:
		case TOKEN_TYPE_VALUE_TYPE:
			this->memory.push(Value(tok.getValue(), tok.getType(), tok.getLine(), tok.getColumn()));
			return ExpressionResult();
		case TOKEN_TYPE_STRING:
			this->memory.push(Value(
				escapeCharacters(tok.getValue()),
				tok.getType(),
				tok.getLine(),
				tok.getColumn()
			));
			return ExpressionResult();
		case TOKEN_TYPE_FSTRING:
			return this->parseFString(tok);
		case TOKEN_TYPE_OPERATOR:
		case TOKEN_TYPE_BOOLEAN_OPERATOR:
			return this->applyOperator(tok);
		case TOKEN_TYPE_AFFECT:
			return this->affectVariable(tok);
		case TOKEN_TYPE_KEYWORD:
			return this->parseKeyword(tok, tokens, previous);
		case TOKEN_TYPE_EXPRESSION_SEPARATOR:
			return ExpressionResult();
		case TOKEN_TYPE_END_OF_LINE:
			this->clearQueue(previous);
			return this->checkMemory();
		case TOKEN_TYPE_COLON:
			return this->parseFunctionCall(tok, tokens);
		case TOKEN_TYPE_INDENT:
			return ExpressionResult();
		default:
			return ExpressionResult(
				"Invalid token '" + tok.getValue() + "'",
				tok.getRange(),
				this->context
			);
	}
}