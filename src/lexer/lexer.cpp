#include "lexer/lexer.hpp"

Lexer::Lexer(std::queue<Token> tokens, const Context *context) : tokens(tokens) {
	this->context = context;
	this->currentLine = new Line();
}

Lexer::~Lexer() {
	delete this->currentLine;
}

/**
 * @brief append current line to the current code block if there is one or to the global code queue otherwise
 */
void Lexer::pushLine() {
	if (this->currentLine->empty()) return;
	if (!this->keywordBlockStack.empty())
		this->keywordBlockStack.top()->push(this->currentLine);
	else
		this->codeBlocks.push(currentLine);

	this->currentLine = new Line();
}

/**
 * @brief lexes the given tokens
 * 
 * @return ExpressionResult if the lexing was successful, otherwise an error
 */
ExpressionResult Lexer::lex() {
	Token token;
	ExpressionResult result;
	while (!this->tokens.empty()) {
		token = this->tokens.front();
		this->tokens.pop();
		switch (token.getType()) {
			case TOKEN_TYPE_END_OF_LINE:
			case TOKEN_TYPE_EXPRESSION_SEPARATOR:
				this->pushLine();
				break;
			case TOKEN_TYPE_FSTRING:
				result = this->parseFString(token);
				break;
			case TOKEN_TYPE_STRING:
				result = this->parseString(token);
				break;
			case TOKEN_TYPE_LITERAL:
				result = this->parseLiteral(token);
				break;
			case TOKEN_TYPE_BIN:
				result = this->parseBinNumber(token);
				break;
			case TOKEN_TYPE_HEX:
				result = this->parseHexNumber(token);
				break;
			default:
				this->currentLine->push(token);
				break;
		}
		if (result.error()) return result;
	}

	if (this->keywordBlockStack.size() > 0)
		return ExpressionResult(
			"Missing closing keyword for block " + this->keywordBlockStack.top()->getKeyword().getValue(),
			token.getRange(),
			this->context
		);
	
	return ExpressionResult();
};

/**
 * @brief convert bin number token to base 10 number token and push it to the current line
 * 
 * @param token the token to convert
 * @return ExpressionResult if the conversion was successful, otherwise an error
 */
ExpressionResult Lexer::parseBinNumber(Token token) {
	std::string value = token.getValue(), result = "";
	int64_t number = 0;
	for (char c : value) {
		number <<= 1;
		number |= c == '1';
	}
	result = std::to_string(number);
	this->currentLine->push(Token(
		token.getLine(),
		token.getColumn(),
		TOKEN_TYPE_INT,
		result
	));
	return ExpressionResult();
}

/**
 * @brief convert hex number token to base 10 number token and push it to the current line
 * 
 * @param token the token to convert
 * @return ExpressionResult the converted token
 */
ExpressionResult Lexer::parseHexNumber(Token token) {
	std::string value = token.getValue(), result = "";
	int64_t number = 0;
	for (char c : value) {
		number <<= 4;
		if (c >= '0' && c <= '9') number |= c - '0';
		else if (c >= 'a' && c <= 'f') number |= c - 'a' + 10;
		else if (c >= 'A' && c <= 'F') number |= c - 'A' + 10;
	}
	result = std::to_string(number);
	this->currentLine->push(Token(
		token.getLine(),
		token.getColumn(),
		TOKEN_TYPE_INT,
		result
	));
	return ExpressionResult();
}

ExpressionResult Lexer::parseFString(Token token) {
	std::string value = token.getValue();
	value = escapeCharacters(value);

	// TODO: Parse fstring

	return ExpressionResult();
}

/**
 * @brief convert all escapes char to their real value and put then into the line
 * 
 * @param token 
 * @return ExpressionResult 
 */
ExpressionResult Lexer::parseString(Token token) {
	token.setValue(escapeCharacters(token.getValue()));
	this->currentLine->push(token);
	return ExpressionResult();
}

ExpressionResult Lexer::parseLiteral(Token token) {
	if (std::regex_match(token.getValue(), keywordsRegex)) {
		token.setType(TOKEN_TYPE_KEYWORD);
		return this->parseKeyword(token);
	}

	this->currentLine->push(token);
	return ExpressionResult();
}

ExpressionResult Lexer::parseKeyword(Token token) {
	if (blockClosers.contains(token.getValue())) {
		if (this->keywordBlockStack.empty()) 
			return ExpressionResult(
				"Expected " + blockClosers.at(token.getValue()) + " token before " + token.getValue(),
				token.getRange(),
				this->context
			);

		CodeBlock *block = this->keywordBlockStack.top();
		this->keywordBlockStack.pop();

		auto* blockCloser = &blockOpeners.at(block->getKeyword().getValue());
		if (std::find(blockCloser->begin(), blockCloser->end(), token.getValue()) == blockCloser->end())
			return ExpressionResult(
				"Expected " + blockClosers.at(block->getKeyword().getValue()) + " token before " + token.getValue(),
				token.getRange(),
				this->context
			);
		
		if (!this->keywordBlockStack.empty())
			this->keywordBlockStack.top()->push(block);
		else
			this->codeBlocks.push(block);
	}

	if (blockOpeners.contains(token.getValue())) {
		this->pushLine();
		this->keywordBlockStack.push(new CodeBlock(token));
		return ExpressionResult();
	}

	this->currentLine->push(token);
	return ExpressionResult();
}

/**
 * @brief take a line of code and convert it to a vector of tokens
 * 
 * @param line the current line number
 * @param lineString the current line of code
 * @param tokens the vector of tokens to fill
 * @return ExpressionResult if the line is a valid expression
 */
ExpressionResult Lexer::tokenize(int line, std::string lineString, std::queue<Token> &tokens, const Context *context) {
	int column = 0;
	std::smatch match;
	while (lineString.size() > 0) {
		int i = 0;
		match = std::smatch();
		while (match.size() == 0 && i < TOKEN_TYPES) {
			std::regex regex = std::get<0>(tokenRegexes[i]);
			if (std::regex_search(lineString, match, regex)) {
				TokenType type = std::get<1>(tokenRegexes[i]);
				std::string value = match.str(1);
				if (type == TOKEN_TYPE_COMMENT) {
					return ExpressionResult();
				}
				if (type != TOKEN_TYPE_INDENT)
					tokens.push(Token(line, column, type, value));
			}
			i++;
		}
		
		if (match.size() == 0) {
			return ExpressionResult(
				"Unexpected char",
				TextRange(line, column, 1),
				context
			);
		}

		column += match.str().size();
		lineString = lineString.substr(match.str().size(), lineString.size() - 1);
		
		while (lineString[0] == ' ') {
			lineString = lineString.substr(1, lineString.size() - 1);
			column++;
		}
	}

	return ExpressionResult();
}