#include "lexer/lexer.hpp"

Lexer::Lexer(std::queue<Token*> tokens, ContextPtr context) :
	context(context),
	currentLine(new Line()),
	tokens(tokens) {}

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

bool Lexer::hasParentKeywordBlock(const std::vector<KeywordEnum> &keywords) const {
	if (keywords.empty()) return true;
	std::stack<CodeBlock*> stack = this->keywordBlockStack;
	while (!stack.empty()) {
		CodeBlock *block = stack.top();
		stack.pop();
		if (std::find(keywords.begin(), keywords.end(), block->getKeyword()) != keywords.end())
			return true;
	}
	return false;
}

BlockQueue& Lexer::getBlocks() {
	return this->codeBlocks;
}

/**
 * @brief lexes the given tokens
 * 
 * @return ExpressionResult if the lexing was successful, otherwise an error
 */
ExpressionResult Lexer::lex() {
	Token *token;
	ExpressionResult result;
	while (!this->tokens.empty()) {
		token = this->tokens.front();
		this->tokens.pop();
		this->integrated = false;
		switch (token->getType()) {
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
			case TOKEN_TYPE_INT:
				this->currentLine->push(new ValueToken(
					new Int(
						std::stoi(token->getStringValue().data()),
						token->getRange(),
						Value::VALUE_TOKEN
					)
				));
				break;
			case TOKEN_TYPE_FLOAT:
				this->currentLine->push(new ValueToken(
					new Float(
						std::stof(token->getStringValue().data()),
						token->getRange(),
						Value::VALUE_TOKEN
					)
				));
				break;
			case TOKEN_TYPE_BOOL:
				this->currentLine->push(new ValueToken(
					new Bool(
						token->getStringValue() == "true",
						token->getRange(),
						Value::VALUE_TOKEN
					)
				));
				break;
			case TOKEN_TYPE_COLON:
				result = this->parseFunctionCall(token);
				break;
			case TOKEN_TYPE_PATH:
				this->currentLine->push(new ValueToken(
					new Path(
						token->getStringValue(),
						token->getRange()
					)
				));
				break;
			case TOKEN_TYPE_VALUE_TYPE:
				this->currentLine->push(new TypeToken(token->getRange(), token->getStringValue()));
				break;
			case TOKEN_TYPE_DOT:
				return ExpressionResult(
					"Expected literal before '.' character",
					token->getRange(),
					this->context
				);
				break;
			case TOKEN_TYPE_OPERATOR:
			case TOKEN_TYPE_BOOLEAN_OPERATOR:
				this->currentLine->push(
					new OperatorToken(token->getRange(), token->getType(), token->getStringValue())
				);
				break;
			default:
				this->integrated = true;
				this->currentLine->push(token);
				break;
		}
		if (result.error()) return result;
		if (!this->integrated) delete token;
	}

	if (this->keywordBlockStack.size() > 0)
		return ExpressionResult(
			"Missing closing keyword for block " + this->keywordBlockStack.top()->getKeywordToken()->getStringValue(),
			token->getRange(),
			this->context
		);
	this->pushLine();

	
	return ExpressionResult();
};

/**
 * @brief convert bin number token to base 10 number token and push it to the current line
 * 
 * @param token the token to convert
 * @return ExpressionResult if the conversion was successful, otherwise an error
 */
ExpressionResult Lexer::parseBinNumber(Token *token) {
	const std::string value = token->getStringValue();
	int64_t number = 0;
	for (char c : value) {
		if (c == 0 && number == 0) continue;
		number <<= 1;
		number |= c == '1';
	}
	this->currentLine->push(new ValueToken(
		new Int(number, token->getRange(), Value::VALUE_TOKEN)
	));
	return ExpressionResult();
}

/**
 * @brief convert hex number token to base 10 number token and push it to the current line
 * 
 * @param token the token to convert
 * @return ExpressionResult the converted token
 */
ExpressionResult Lexer::parseHexNumber(Token *token) {
	const std::string value = token->getStringValue();
	int64_t number = 0;
	for (char c : value) {
		number <<= 4;
		if (c >= '0' && c <= '9') number |= c - '0';
		else if (c >= 'a' && c <= 'f') number |= c - 'a' + 10;
		else if (c >= 'A' && c <= 'F') number |= c - 'A' + 10;
	}
	this->currentLine->push(new ValueToken(
		new Int(number, token->getRange(), Value::VALUE_TOKEN)
	));
	return ExpressionResult();
}

ExpressionResult Lexer::parseFString(Token *token) {
	const std::string value = escapeCharacters(std::string(token->getStringValue()));

	std::vector<std::string> parts;
	parts.push_back("");
	for (auto it = value.begin(); it != value.end(); it++) {
		if (*it == '{') {
			if (it == value.end() || *(it + 1) != '}')
				return ExpressionResult(
					"Invalid fstring format: missing closing bracket",
					TextRange(
						token->getRange().line, 
						token->getRange().columnStart + (it - value.begin()),
						1
					),
					this->context
				);
			parts.push_back("");
			it++;
		} else if (*it == '}') {
			return ExpressionResult(
				"Invalid fstring format: missing opening bracket",
				TextRange(
					token->getRange().line, 
					token->getRange().columnStart + (it - value.begin()),
					1
				),
				this->context
			);
		} else {
			parts.back() += *it;
		}
	}
	
	this->currentLine->push(new FStringToken(
		token->getRange(),
		parts
	));

	return ExpressionResult();
}

/**
 * @brief convert all escapes char to their real value and put then into the line
 * 
 * @param token 
 * @return ExpressionResult 
 */
ExpressionResult Lexer::parseString(Token *token) {
	this->currentLine->push(new ValueToken(
		new String(
			escapeCharacters(token->getStringValue()),
			token->getRange(),
			Value::VALUE_TOKEN
		)
	));
	return ExpressionResult();
}

/**
 * @brief check if the tokens is a keyword and otherwise convert it to a variable token
 * 
 * @param token the token to convert
 * @return ExpressionResult if the conversion was successful, otherwise an error
 */
ExpressionResult Lexer::parseLiteral(Token *token) {
	if (keywordsRegex(token->getStringValue())) {
		token = new KeywordToken(token->getRange(), token->getStringValue());
		ExpressionResult result =  this->parseKeyword(token);
		if (!this->integrated) delete token;
		this->integrated = false;
		return result;
	}

	if (this->tokens.size() > 0 && this->tokens.front()->getType() == TOKEN_TYPE_DOT) {
		return this->parsePath(token);
	}

	this->currentLine->push(new ValueToken(
		new Variable(
			token->getStringValue(),
			token->getRange()
		),
		TOKEN_TYPE_LITERAL
	));
	return ExpressionResult();
}

/**
 * @brief check if a given path is correctly formatted and convert it to a path token
 * 
 * @param token the first literal of the path
 * @return ExpressionResult if the conversion was successful, otherwise an error
 */
ExpressionResult Lexer::parsePath(Token *token) {
	std::vector<std::string> path{token->getStringValue()};
	while (this->tokens.size() > 0 && this->tokens.front()->getType() == TOKEN_TYPE_DOT) {
		delete this->tokens.front();
		this->tokens.pop();
		if (this->tokens.size() == 0 || this->tokens.front()->getType() != TOKEN_TYPE_LITERAL)
			return ExpressionResult(
				"Invalid path: expected literal after dot",
				token->getRange(),
				this->context
			);
		path.push_back(this->tokens.front()->getStringValue());
		delete this->tokens.front();
		this->tokens.pop();
	}
	this->currentLine->push(new ValueToken(
		new Path(path, token->getRange()),
		TOKEN_TYPE_PATH
	));
	return ExpressionResult();
}

/**
 * @brief parse the given keyword token and manage each token usage
 * 
 * @param token the token to check
 * @return ExpressionResult if the token was valid, otherwise an error
 */
ExpressionResult Lexer::parseKeyword(Token *token) {
	KeywordEnum tokenKeyword = static_cast<KeywordToken *>(token)->getKeyword();
	// all checks for validity are made when a closing block is encounter
	if (blockClosers.contains(tokenKeyword)) {
		if (this->keywordBlockStack.empty()) 
			return ExpressionResult(
				"Expected " + KeywordToken::keywordString(blockClosers.at(tokenKeyword)) + " token before " + token->getStringValue(),
				token->getRange(),
				this->context
			);

		BaseBlock *block = this->keywordBlockStack.top();
		const KeywordEnum name = this->keywordBlockStack.top()->getKeyword();
		this->keywordBlockStack.pop();
		const std::vector<KeywordEnum> &blockCloser = blockOpeners.at(name);
		if (std::find(blockCloser.begin(), blockCloser.end(), tokenKeyword) == blockCloser.end())
			return ExpressionResult(
				"Expected " + KeywordToken::keywordString(blockClosers.at(tokenKeyword)) + " token before " + token->getStringValue(),
				token->getRange(),
				this->context
			);
		
		if (name == KEYWORD_FUN) {
			auto [result, function] = this->parseFunction(static_cast<CodeBlock*>(block));
			if (result.error()) return result;
			block = function;
		} else if (name == KEYWORD_STRUCT) {
			auto [result, structDefinition] = this->parseStruct(static_cast<CodeBlock*>(block));
			if (result.error()) return result;
			block = structDefinition;
		}

		if (!this->keywordBlockStack.empty()) {
			if (blockClosers.contains(name))
				static_cast<CodeBlock*>(
					this->keywordBlockStack.top()->getBlocks().back()
				)->setNext(static_cast<CodeBlock*>(block));
			else
				this->keywordBlockStack.top()->push(block);
		} else if (blockClosers.contains(name)){
			static_cast<CodeBlock*>(this->codeBlocks.back())->setNext(static_cast<CodeBlock*>(block));
		} else {
			this->codeBlocks.push(block);
		}
	}

	// open a new block when encounter a block keyword like if, fun, while...
	if (blockOpeners.contains(tokenKeyword)) {
		this->pushLine();
		this->keywordBlockStack.push(new CodeBlock(static_cast<KeywordToken*>(token)));
		this->integrated = true;
		return ExpressionResult();
	}

	// parent dependancy is the fact that some tokens need to be in a block to be valid
	// ex: continue must be in a loop
	if (parentDependency.contains(tokenKeyword)) {
		if (!hasParentKeywordBlock(parentDependency.at(tokenKeyword))) {
			std::string result = token->getStringValue() + "must be inside of ";
			for (const KeywordEnum &keyword : parentDependency.at(tokenKeyword)) {
				result += KeywordToken::keywordString(keyword) + " or ";
			}
			result.erase(result.end() - 4, result.end());
			result += " block";
			return ExpressionResult(
				result,
				token->getRange(),
				this->context
			);
		}
		this->currentLine->push(token);
		this->integrated = true;
	}

	return ExpressionResult();	
}

/**
 * @brief check if the function call is correctlly formed and convert it to a function call token
 * 
 * @param token the colon token (before the function name)
 * @return ExpressionResult if the conversion was successful, otherwise an error
 */
ExpressionResult Lexer::parseFunctionCall(const Token *token) {
	if (
		this->tokens.empty() || this->tokens.front()->getType() != TOKEN_TYPE_LITERAL
	) {
		return ExpressionResult(
			"Expected function name after colon token",
			this->tokens.empty() ? token->getRange() : token->getRange().merge(this->tokens.front()->getRange()),
			this->context
		);
	}
	
	Token *literal = this->tokens.front();
	this->tokens.pop();
	ExpressionResult result = this->parseLiteral(literal);
	delete literal;
	if (result.error()) return result;
	if (this->currentLine->back()->getType() == TOKEN_TYPE_PATH) {
		this->currentLine->back()->setType(TOKEN_TYPE_MODULE_FUNCTION_CALL);
	} else {	
		this->currentLine->back()->setType(TOKEN_TYPE_FUNCTION_CALL);
	}

	return ExpressionResult();
}

/**
 * @brief Check if the previous line is a function declaration and convert it to a function token
 * 
 * @param keyword the fun keyword token
 * @return ExpressionResult if the conversion was successful, otherwise an error
 */
std::pair<ExpressionResult, FunctionBlock*> Lexer::parseFunction(CodeBlock *block) {
	const Token *keyword = block->getKeywordToken();
	if (this->codeBlocks.empty())
		return std::make_pair(ExpressionResult(
			"Expected function definition before function block",
			keyword->getRange(),
			this->context
		), nullptr);
	
	std::vector<std::string>args;
	std::vector<ValueType>types;

	std::unique_ptr<Line>line {static_cast<Line*>(this->codeBlocks.popBack())};
	if (line->size() < 3)
		return std::make_pair(ExpressionResult(
			"Expected function name and return type before fun keyword",
			keyword->getRange(),
			this->context
		), nullptr);
	if (line->top()->getType() != TOKEN_TYPE_LITERAL)
		return std::make_pair(ExpressionResult(
			"Expected function name before fun keyword",
			line->top()->getRange(),
			this->context
		), nullptr);
	
	const std::string name = line->pop()->getStringValue();
	int i = 0;
	Token *current = nullptr;
	while (line->top()->getType() != TOKEN_TYPE_ARROW && line->size() > 2) {
		current = line->pop();
		if (i % 2 == 0) {
			if (current->getType() != TOKEN_TYPE_VALUE_TYPE)
				return std::make_pair(ExpressionResult(
					"Expected value type",
					current->getRange(),
					this->context
				), nullptr);
			types.push_back(static_cast<TypeToken*>(current)->getValueType());
		} else {
			if (current->getType() != TOKEN_TYPE_LITERAL)
				return std::make_pair(ExpressionResult(
					"Expected argument name",
					current->getRange(),
					this->context
				), nullptr);
			args.push_back(current->getStringValue());
		}
		i++;
	}
	if (line->top()->getType() != TOKEN_TYPE_ARROW)
		return std::make_pair(ExpressionResult(
			"Missing '->' token before return type",
			line->top()->getRange(),
			this->context 
		), nullptr);
	if (i % 2 != 0)
		return std::make_pair(ExpressionResult(
			"Missing argument name after argument type",
			current->getRange(),
			this->context
		), nullptr);
	line->pop();
	if (line->top()->getType() != TOKEN_TYPE_VALUE_TYPE)
		return std::make_pair(ExpressionResult(
			"Return type expected after '->' token but got" + line->top()->getStringType(),
			line->top()->getRange(),
			this->context
		), nullptr);
	
	ValueType returnType = Value::valueType(line->pop()->getStringValue());

	return std::make_pair(ExpressionResult(), new FunctionBlock(
		name,
		args,
		types,
		returnType,
		block
	));
}

/**
 * @brief Check if the previous line containts a struct name and parse the struct definition
 * 
 * For example:
 * Exemple struct
 * 		name -> string
 * 		age -> int
 * 		...
 * tcurts
 * 
 * @param block the struct definition block
 * @return std::pair<ExpressionResult, StructBlock*> if the struct is correct and the struct block
 */
std::pair<ExpressionResult, StructBlock*> Lexer::parseStruct(CodeBlock *block) {
	if (this->codeBlocks.empty()) {
		return std::make_pair(ExpressionResult(
			"Exepcted struct name before struct block",
			block->getRange(),
			this->context
		), nullptr);
	}
	std::unique_ptr<Line>line {static_cast<Line*>(this->codeBlocks.popBack())};
	if (line->size() < 1) {
		return std::make_pair(ExpressionResult(
			"Expected struct name before struct block",
			block->getRange(),
			this->context
		), nullptr);
	}
	if (line->size() != 1) {
		return std::make_pair(ExpressionResult(
			"Expected only struct name before struct block",
			line->top()->getRange(),
			this->context
		), nullptr);
	}
	if (line->top()->getType() != TOKEN_TYPE_STRUCT_NAME) {
		return std::make_pair(ExpressionResult(
			"Expected struct name before struct block",
			line->top()->getRange(),
			this->context
		), nullptr);
	}

	std::string_view name = line->top()->getStringValue();
	StructDefinition def(name);
	for (BaseBlock *b : *block) {
		if (b->getType() != LINE_BLOCK) {
			return std::make_pair(ExpressionResult(
				"Struct member definition must be in the form 'name -> type'",
				b->lastRange(),
				this->context
			), nullptr);
		}
		Line *l = static_cast<Line *>(b);
		if (l->size() != 3) {
			return std::make_pair(ExpressionResult(
				"Struct member definition must be in the form 'name -> type'",
				l->size() != 0 ? l->lineRange() : line->top()->getRange(),
				this->context
			), nullptr);
		}
		if (l->top()->getType() != TOKEN_TYPE_LITERAL) {
			return std::make_pair(ExpressionResult(
				"Struct member name must be a literal",
				l->top()->getRange(),
				this->context
			), nullptr);
		}
		std::unique_ptr<Token> name{l->pop()};
		if (l->top()->getType() != TOKEN_TYPE_ARROW) {
			return std::make_pair(ExpressionResult(
				"Struct member definition must be in the form 'name -> type'",
				l->top()->getRange(),
				this->context
			), nullptr);
		}
		delete l->pop();
		if (l->top()->getType() != TOKEN_TYPE_VALUE_TYPE) {
			return std::make_pair(ExpressionResult(
				"Struct member type must be a value type",
				l->top()->getRange(),
				this->context
			), nullptr);
		}
		std::unique_ptr<TypeToken> type{static_cast<TypeToken*>(l->pop())};
		def.addMember(name->getStringValue(), type->getValueType());
	}
	return std::make_pair(ExpressionResult(), new StructBlock(def));
}

/**
 * @brief take a line of code and convert it to a vector of tokens
 * 
 * @param lineNumber the current line number
 * @param lineString the current line of code
 * @param tokens the vector of tokens to fill
 * @return ExpressionResult if the line is a valid expression
 */
ExpressionResult Lexer::tokenize(int lineNumber, std::string_view lineString, std::queue<Token*> &tokens, const ContextPtr &context) {
	unsigned int column = 0;
	std::string_view value;
	matchResult result;
	size_t i, matchSize;
	bool match;
	while (lineString.size() > 0) {
		i = 0;
		match = false;
		while (!match && i < tokenRegexes.size()) {
			Matcher regex = std::get<0>(tokenRegexes[i]);
			if ((result = regex(lineString.data())).has_value()) {
				match = true;
				value = result.value().first;
				matchSize = result.value().second;
				
				TokenType type = std::get<1>(tokenRegexes[i]);
				if (type == TOKEN_TYPE_COMMENT)
					return ExpressionResult();
				if (type != TOKEN_TYPE_INDENT)
					tokens.push(new StringToken(lineNumber, column, type, value));
				else
					column += 1;
			}
			i++;
		}
		
		if (!match) {
			return ExpressionResult(
				"Unexpected char : '" + std::string(1, lineString[0]) + "'",
				TextRange(lineNumber, column, 1),
				context
			);
		}

		column += matchSize;
		lineString = lineString.substr(matchSize, lineString.size() - 1);
		
		// remove spaces
		while (lineString.size() > 0 && lineString[0] == ' ') {
			lineString = lineString.substr(1, lineString.size() - 1);
			column++;
		}
	}
	return ExpressionResult();
}