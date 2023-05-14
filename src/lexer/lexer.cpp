#include "lexer/lexer.hpp"

Lexer::Lexer(const std::deque<Token *> &tokens, ContextPtr context)
	: context(context), tokens(tokens) {}

Lexer::~Lexer() {
	delete this->currentLine;
}

/**
 * @brief append current line to the current code block if there is one or to the global code queue
 * otherwise
 */
void Lexer::pushLine() {
	if (this->currentLine->empty()) {
		return;
	}
	if (!this->keywordBlockStack.empty()) {
		this->keywordBlockStack.top()->push(this->currentLine);
	} else {
		this->codeBlocks.push(this->currentLine);
	}

	this->currentLine = new Line();
}

bool Lexer::hasParentKeywordBlock(const std::vector<KeywordEnum> &keywords) const {
	if (keywords.empty()) {
		return true;
	}
	std::stack<CodeBlock *> stack = this->keywordBlockStack;
	while (!stack.empty()) {
		CodeBlock const *block = stack.top();
		stack.pop();
		if (std::ranges::find(keywords, block->getKeyword()) != keywords.end()) {
			return true;
		}
	}
	return false;
}

BlockQueue &Lexer::getBlocks() {
	return this->codeBlocks;
}

/**
 * @brief lexes the given tokens
 *
 * @return ExpressionResult if the lexing was successful, otherwise an error
 */
ExpressionResult Lexer::lex() {
	Token *token = nullptr;
	ExpressionResult result;
	while (!this->tokens.empty()) {
		token = this->tokens.front();
		this->tokens.pop_front();
		this->integrated = false;
		switch (token->getType()) {
			case TokenType::TOKEN_TYPE_END_OF_LINE:
			case TokenType::TOKEN_TYPE_EXPRESSION_SEPARATOR:
				this->pushLine();
				break;
			case TokenType::TOKEN_TYPE_FSTRING:
				result = this->parseFString(token);
				break;
			case TokenType::TOKEN_TYPE_STRING:
				result = this->parseString(token);
				break;
			case TokenType::TOKEN_TYPE_LITERAL:
				result = this->parseLiteral(token);
				break;
			case TokenType::TOKEN_TYPE_BIN:
				result = this->parseBinNumber(token);
				break;
			case TokenType::TOKEN_TYPE_HEX:
				result = this->parseHexNumber(token);
				break;
			case TokenType::TOKEN_TYPE_INT:
				this->currentLine->push(new ValueToken(new Int(
					std::stoi(token->getStringValue()), token->getRange(), Value::VALUE_TOKEN)));
				break;
			case TokenType::TOKEN_TYPE_FLOAT:
				this->currentLine->push(new ValueToken(new Float(
					std::stof(token->getStringValue()), token->getRange(), Value::VALUE_TOKEN)));
				break;
			case TokenType::TOKEN_TYPE_BOOL:
				this->currentLine->push(new ValueToken(new Bool(
					token->getStringValue() == "true", token->getRange(), Value::VALUE_TOKEN)));
				break;
			case TokenType::TOKEN_TYPE_COLON:
				result = this->parseFunctionCall(token);
				break;
			case TokenType::TOKEN_TYPE_VALUE_TYPE:
				result = this->parseType(token);
				break;
			case TokenType::TOKEN_TYPE_DOT:
				return {"Expected literal before '.' character", token->getRange(), this->context};
				break;
			case TokenType::TOKEN_TYPE_ARROW:
				result = this->parseStructAccess(token);
				break;
			case TokenType::TOKEN_TYPE_OPERATOR:
			case TokenType::TOKEN_TYPE_BOOLEAN_OPERATOR:
				this->currentLine->push(new OperatorToken(token->getRange(), token->getType(),
														  token->getStringValue()));
				break;
			default:
				this->integrated = true;
				this->currentLine->push(token);
				break;
		}
		if (result.error()) {
			return result;
		}
		if (!this->integrated) {
			delete token;
		}
	}

	if (!this->keywordBlockStack.empty()) {
		return {"Missing closing keyword for block " +
					this->keywordBlockStack.top()->getKeywordToken()->getStringValue(),
				token->getRange(), this->context};
	}

	this->pushLine();
	Analyzer analyzer(this->context);
	analyzer.analyze(this->codeBlocks, true);
	return analyzer.analyzeErrors();
};

/**
 * @brief convert bin number token to base 10 number token and push it to the current line
 *
 * @param token the token to convert
 * @return ExpressionResult if the conversion was successful, otherwise an error
 */
ExpressionResult Lexer::parseBinNumber(Token const *token) {
	const std::string value = token->getStringValue();
	int64_t number = 0;
	for (char const c : value) {
		if (c == 0 && number == 0) {
			continue;
		}
		number <<= 1;
		number |= static_cast<long>(c == '1');
	}
	this->currentLine->push(new ValueToken(new Int(number, token->getRange(), Value::VALUE_TOKEN)));
	return {};
}

/**
 * @brief convert hex number token to base 10 number token and push it to the current line
 *
 * @param token the token to convert
 * @return ExpressionResult the converted token
 */
ExpressionResult Lexer::parseHexNumber(Token const *token) {
	const std::string value = token->getStringValue();
	int64_t number = 0;
	for (char const c : value) {
		number <<= 4;
		if (c >= '0' && c <= '9') {
			number |= c - '0';
		} else if (c >= 'a' && c <= 'f') {
			number |= c - 'a' + 10;
		} else if (c >= 'A' && c <= 'F') {
			number |= c - 'A' + 10;
		}
	}
	this->currentLine->push(new ValueToken(new Int(number, token->getRange(), Value::VALUE_TOKEN)));
	return {};
}

ExpressionResult Lexer::parseFString(Token const *token) {
	const std::string value = escapeCharacters(std::string(token->getStringValue()));

	std::vector<std::string> parts;
	parts.emplace_back("");
	auto it = value.begin();
	while (it != value.end()) {
		if (*it == '{') {
			if (it == value.end() || *(it + 1) != '}') {
				return {"Invalid fstring format: missing closing bracket",
						TextRange(token->getRange().line,
								  token->getRange().columnStart + (it - value.begin()), 1),
						this->context};
			}
			parts.emplace_back("");
			it++;
		} else if (*it == '}') {
			return {"Invalid fstring format: missing opening bracket",
					TextRange(token->getRange().line,
							  token->getRange().columnStart + (it - value.begin()), 1),
					this->context};
		} else {
			parts.back() += *it;
		}
		it++;
	}

	this->currentLine->push(new FStringToken(token->getRange(), parts));

	return {};
}

/**
 * @brief convert all escapes char to their real value and put then into the line
 *
 * @param token
 * @return ExpressionResult
 */
ExpressionResult Lexer::parseString(Token const *token) {
	this->currentLine->push(new ValueToken(new String(escapeCharacters(token->getStringValue()),
													  token->getRange(), Value::VALUE_TOKEN)));
	return {};
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
		ExpressionResult result = this->parseKeyword(token);
		if (!this->integrated) {
			delete token;
		}
		this->integrated = false;
		return result;
	}

	if (!this->tokens.empty()) {
		Token const *next = this->tokens.front();
		if (next->getType() == TokenType::TOKEN_TYPE_DOT) {
			return this->parsePath(token);
		}
	}

	this->currentLine->push(new ValueToken(new Variable(token->getStringValue(), token->getRange()),
										   TokenType::TOKEN_TYPE_LITERAL));
	return {};
}

/**
 * @brief check if a given path is correctly formatted and convert it to a path token
 *
 * @param token the first literal of the path
 * @return ExpressionResult if the conversion was successful, otherwise an error
 */
ExpressionResult Lexer::parsePath(Token const *token) {
	std::vector<std::string> path{token->getStringValue()};
	TextRange range = token->getRange();
	while (!this->tokens.empty() && this->tokens.front()->getType() == TokenType::TOKEN_TYPE_DOT) {
		delete this->tokens.front();
		this->tokens.pop_front();
		if (this->tokens.empty() || this->tokens.front()->getType() != TokenType::TOKEN_TYPE_LITERAL) {
			return {"Invalid path: expected literal after dot", token->getRange(), this->context};
		}
		path.push_back(this->tokens.front()->getStringValue());
		range.merge(this->tokens.front()->getRange());
		delete this->tokens.front();
		this->tokens.pop_front();
	}
	this->currentLine->push(new ValueToken(new Path(path, range), TokenType::TOKEN_TYPE_PATH));
	return {};
}

/**
 * @brief check if a given struct access is correctly formatted : ->name[->name...]
 *
 * @param token the first arrow of the struct access
 * @return ExpressionResult if the conversion was successful, otherwise an error
 */
ExpressionResult Lexer::parseStructAccess(Token *token) {
	std::vector<std::string> path;
	TextRange range = token->getRange();
	if (this->tokens.empty()) {
		return {"Invalid struct access: expected value before arrow", token->getRange(),
				this->context};
	}
	if (this->tokens.front()->getType() == TokenType::TOKEN_TYPE_VALUE_TYPE ||
		this->tokens.front()->getType() == TokenType::TOKEN_TYPE_STRUCT_NAME) {
		this->integrated = true;
		this->currentLine->push(token);
		return {};
	}
	if (this->tokens.front()->getType() != TokenType::TOKEN_TYPE_LITERAL) {
		return {"Invalid struct access: expected literal before arrow", token->getRange(),
				this->context};
	}
	path.push_back(this->tokens.front()->getStringValue());
	delete this->tokens.front();
	this->tokens.pop_front();
	TextRange arrowRange = token->getRange();
	while (!this->tokens.empty() && this->tokens.front()->getType() == TokenType::TOKEN_TYPE_ARROW) {
		arrowRange = this->tokens.front()->getRange();
		delete this->tokens.front();
		this->tokens.pop_front();
		if (this->tokens.empty() || this->tokens.front()->getType() != TokenType::TOKEN_TYPE_LITERAL) {
			return {"Invalid struct access: expected literal after arrow", arrowRange,
					this->context};
		}
		path.push_back(this->tokens.front()->getStringValue());
		range.merge(this->tokens.front()->getRange());
		delete this->tokens.front();
		this->tokens.pop_front();
	}

	this->currentLine->push(
		new ValueToken(new Path(path, range, STRUCT_ACCESS), TokenType::TOKEN_TYPE_STRUCT_ACCESS));
	return {};
}

/**
 * @brief parse the given keyword token and manage each token usage
 *
 * @param token the token to check
 * @return ExpressionResult if the token was valid, otherwise an error
 */
ExpressionResult Lexer::parseKeyword(Token *token) {
	KeywordEnum const tokenKeyword = dynamic_cast<KeywordToken *>(token)->getKeyword();
	// all checks for validity are made when a closing block is encounter
	if (blockClosers.contains(tokenKeyword)) {
		if (this->keywordBlockStack.empty()) {
			return {"Expected " + KeywordToken::keywordString(blockClosers.at(tokenKeyword)) +
						" token before " + token->getStringValue(),
					token->getRange(), this->context};
		}

		BaseBlock *block = this->keywordBlockStack.top();
		const KeywordEnum name = this->keywordBlockStack.top()->getKeyword();
		this->keywordBlockStack.pop();
		const std::vector<KeywordEnum> &blockCloser = blockOpeners.at(name);
		if (std::ranges::find(blockCloser, tokenKeyword) == blockCloser.end()) {
			return {"Expected " + KeywordToken::keywordString(blockClosers.at(tokenKeyword)) +
						" token before " + token->getStringValue(),
					token->getRange(), this->context};
		}

		if (name == KEYWORD_FUN) {
			auto [result, function] = this->parseFunction(dynamic_cast<CodeBlock *>(block));
			if (result.error()) {
				return result;
			}
			block = function;
		} else if (name == KEYWORD_STRUCT) {
			ExpressionResult result = this->parseStruct(dynamic_cast<CodeBlock *>(block));
			delete block;
			return result;
		}

		if (!this->keywordBlockStack.empty()) {
			if (blockClosers.contains(name)) {
				dynamic_cast<CodeBlock *>(this->keywordBlockStack.top()->getBlocks().back())
					->setNext(dynamic_cast<CodeBlock *>(block));
			} else {
				this->keywordBlockStack.top()->push(block);
			}
		} else if (blockClosers.contains(name)) {
			dynamic_cast<CodeBlock *>(this->codeBlocks.back())
				->setNext(dynamic_cast<CodeBlock *>(block));
		} else {
			this->codeBlocks.push(block);
		}
	}

	// open a new block when encounter a block keyword like if, fun, while...
	if (blockOpeners.contains(tokenKeyword)) {
		this->pushLine();
		if (!this->keywordBlockStack.empty() &&
			this->keywordBlockStack.top()->getKeyword() == KEYWORD_STRUCT) {
			return {"Structs cannot contain other blocks", token->getRange(), this->context};
		}
		this->keywordBlockStack.push(new CodeBlock(dynamic_cast<KeywordToken *>(token)));
		this->integrated = true;
		return {};
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
			return {result, token->getRange(), this->context};
		}
		if (tokenKeyword == KEYWORD_FUNSIG) {
			return this->parseFunctionSignature(token);	
		} else {
			this->currentLine->push(token);
			this->integrated = true;
		}
	}

	return {};
}

/**
 * @brief check if the function call is correctlly formed and convert it to a function call token
 *
 * @param token the colon token (before the function name)
 * @return ExpressionResult if the conversion was successful, otherwise an error
 */
ExpressionResult Lexer::parseFunctionCall(Token const *token) {
	if (this->tokens.empty() || this->tokens.front()->getType() != TokenType::TOKEN_TYPE_LITERAL) {
		return {"Expected function name after colon token",
				this->tokens.empty() ? token->getRange()
									 : token->getRange().merge(this->tokens.front()->getRange()),
				this->context};
	}

	Token *literal = this->tokens.front();
	this->tokens.pop_front();
	TextRange const literalRange = literal->getRange();
	ExpressionResult result = this->parseLiteral(literal);
	delete literal;
	if (result.error()) {
		return result;
	}
	if (this->currentLine->empty() || (this->currentLine->back()->getType() != TokenType::TOKEN_TYPE_LITERAL &&
									   this->currentLine->back()->getType() != TokenType::TOKEN_TYPE_PATH)) {
		return {"Expected function name after colon token", token->getRange().merge(literalRange),
				this->context};
	}
	if (this->currentLine->back()->getType() == TokenType::TOKEN_TYPE_PATH) {
		this->currentLine->back()->setType(TokenType::TOKEN_TYPE_MODULE_FUNCTION_CALL);
	} else {
		this->currentLine->back()->setType(TokenType::TOKEN_TYPE_FUNCTION_CALL);
	}

	return {};
}

/**
 * @brief Check if the previous line is a function declaration and convert it to a function token
 *
 * @param keyword the fun keyword token
 * @return ExpressionResult if the conversion was successful, otherwise an error
 */
std::pair<ExpressionResult, FunctionBlock *> Lexer::parseFunction(CodeBlock *block) {
	const Token *keyword = block->getKeywordToken();
	if (this->codeBlocks.empty()) {
		return std::make_pair(ExpressionResult("Expected function definition before function block",
											   keyword->getRange(), this->context),
							  nullptr);
	}

	std::unique_ptr<Line> line{dynamic_cast<Line *>(this->codeBlocks.popBack())};
	if (line->size() < 3) {
		return std::make_pair(
			ExpressionResult("Expected function name and return type before fun keyword",
							 keyword->getRange(), this->context),
			nullptr);
	}
	if (line->top()->getType() != TokenType::TOKEN_TYPE_LITERAL) {
		return std::make_pair(ExpressionResult("Expected function name before fun keyword",
											   line->top()->getRange(), this->context),
							  nullptr);
	}

	const std::string name = line->pop()->getStringValue();
	int i = 0;
	Token *current = nullptr;
	RPNFunctionArgs types;
	std::pair<std::string, RPNValueType> type;
	while (line->top()->getType() != TokenType::TOKEN_TYPE_ARROW && line->size() > 2) {
		current = line->pop();
		if (i % 2 == 0) {
			if (current->getType() == TokenType::TOKEN_TYPE_VALUE_TYPE) {
				type.second = dynamic_cast<TypeToken *>(current)->getValueType();
			} else if (current->getType() == TokenType::TOKEN_TYPE_STRUCT_NAME || current->getType() == TokenType::TOKEN_TYPE_FUN_SIGNATURE) {
				type.second = current->getStringValue();
			} else {
				return std::make_pair(ExpressionResult("Expected value type or struct name",
													   current->getRange(), this->context),
									  nullptr);
			}
		} else {
			if (current->getType() != TokenType::TOKEN_TYPE_LITERAL) {
				return std::make_pair(
					ExpressionResult("Expected argument name", current->getRange(), this->context),
					nullptr);
			}
			type.first = current->getStringValue();
			types.push_back(type);
		}
		i++;
	}
	if (line->top()->getType() != TokenType::TOKEN_TYPE_ARROW) {
		return std::make_pair(ExpressionResult("Missing '->' token before return type",
											   line->top()->getRange(), this->context),
							  nullptr);
	}
	if (i % 2 != 0) {
		return std::make_pair(ExpressionResult("Missing argument name after argument type",
											   current->getRange(), this->context),
							  nullptr);
	}
	line->pop();
	if (line->top()->getType() == TokenType::TOKEN_TYPE_VALUE_TYPE) {
		return std::make_pair(
			ExpressionResult(),
			new FunctionBlock(name, types, dynamic_cast<TypeToken *>(line->pop())->getValueType(),
							  block));
	}
	if (line->top()->getType() == TokenType::TOKEN_TYPE_STRUCT_NAME) {
		return std::make_pair(ExpressionResult(),
							  new FunctionBlock(name, types, line->pop()->getStringValue(), block));
	}

	return std::make_pair(ExpressionResult("Return type expected after '->' token but got" +
											   line->top()->getStringType(),
										   line->top()->getRange(), this->context),
						  nullptr);
}

/**
 * @brief Parse function signature
 * A function signature is a line of the form:
 * name argType1, argType2, ... -> return_type funsig
 * 
 * @param token the funsig token
 * @return ExpressionResult  if the function signature is correct
 */
ExpressionResult Lexer::parseFunctionSignature(Token *token) {
	if (this->currentLine->empty()) {
		return {"Expected function definition before funsig token", token->getRange(), this->context};
	}
	if (this->currentLine->top()->getType() != TokenType::TOKEN_TYPE_LITERAL) {
		return {"Expected function name before funsig token", token->getRange(), this->context};
	}
	std::string name = this->currentLine->pop()->getStringValue();
	std::vector<RPNValueType> types;
	
	while (this->currentLine->top()->getType() != TokenType::TOKEN_TYPE_ARROW) {
		if (this->currentLine->top()->getType() == TokenType::TOKEN_TYPE_VALUE_TYPE) {
			types.emplace_back(dynamic_cast<TypeToken *>(this->currentLine->pop())->getValueType());
		} else if (this->currentLine->top()->getType() == TokenType::TOKEN_TYPE_STRUCT_NAME || this->currentLine->top()->getType() == TokenType::TOKEN_TYPE_FUN_SIGNATURE) {
			types.emplace_back(this->currentLine->pop()->getStringValue());
		} else {
			return {"Expected argument type or struct name before arrow token",
					this->currentLine->top()->getRange(), this->context};
		}
		if (this->currentLine->empty()) {
			return {"Expected arrow token after function arguments", token->getRange(), this->context};
		}
	}
	this->currentLine->pop();
	if (this->currentLine->empty()) {
		return {"Expected return type after arrow token", token->getRange(), this->context};
	}
	RPNValueType returnType;
	if (this->currentLine->top()->getType() == TokenType::TOKEN_TYPE_VALUE_TYPE) {
		returnType = dynamic_cast<TypeToken *>(this->currentLine->pop())->getValueType();
	} else if (this->currentLine->top()->getType() == TokenType::TOKEN_TYPE_STRUCT_NAME) {
		returnType = this->currentLine->pop()->getStringValue();
	} else {
		return {"Expected return type after arrow token", token->getRange(), this->context};
	}
	if (!this->currentLine->empty()) {
		return {"Unexpected token after function signature", this->currentLine->top()->getRange(), this->context};
	}
	this->codeBlocks.push(
		new FunctionSignatureLine(FunctionSignature{types, returnType, false, false}, name, token->getRange()));
	return {};
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
 * @return ExpressionResult if the struct is correct and the struct block
 */
ExpressionResult Lexer::parseStruct(CodeBlock *block) {
	if (this->codeBlocks.empty()) {
		return {"Exepcted struct name before struct block", block->getRange(), this->context};
	}
	std::unique_ptr<Line> line{dynamic_cast<Line *>(this->codeBlocks.popBack())};
	if (line->empty()) {
		return {"Expected struct name before struct block", block->getRange(), this->context};
	}
	if (line->size() != 1) {
		return {"Expected only struct name before struct block", line->top()->getRange(),
				this->context};
	}
	if (line->top()->getType() != TokenType::TOKEN_TYPE_STRUCT_NAME) {
		return {"Expected struct name before struct block", line->top()->getRange(), this->context};
	}

	StructDefinition def(line->top()->getStringValue());
	if (block->empty()) {
		return {"Struct definition must contain at least one member", block->getRange(),
				this->context};
	}
	for (BaseBlock *b : *block) {
		Line *l = dynamic_cast<Line *>(b);
		if (l->size() != 3) {
			return {"Struct member definition must be in the form 'name -> type'",
					!l->empty() ? l->lineRange() : line->top()->getRange(), this->context};
		}
		if (l->top()->getType() != TokenType::TOKEN_TYPE_LITERAL) {
			return {"Struct member name must be a literal", l->top()->getRange(), this->context};
		}
		Token const *nameToken = l->pop();
		if (l->top()->getType() != TokenType::TOKEN_TYPE_ARROW) {
			return {"Struct member definition must be in the form 'name -> type'",
					l->top()->getRange(), this->context};
		}
		l->pop();
		if (l->top()->getType() != TokenType::TOKEN_TYPE_VALUE_TYPE) {
			return {"Struct member type must be a value type", l->top()->getRange(), this->context};
		}
		def.addMember(nameToken->getStringValue(),
					  dynamic_cast<TypeToken *>(l->pop())->getValueType());
	}
	Struct::addStructDefinition(def);
	return {};
}

ExpressionResult Lexer::parseType(const Token *token) {
	this->currentLine->push(new TypeToken(token->getRange(), token->getStringValue()));
	RPNValueType type = dynamic_cast<TypeToken *>(this->currentLine->back())->getValueType();
	if (std::get<ValueType>(type.getType()) != LIST) {
		return {};
	}
	if (this->tokens.size() < 3) {
		return {"List type require a content type in the form 'list[type]'", token->getRange(),
				this->context};
	}
	if (this->tokens.front()->getType() != TokenType::TOKEN_TYPE_LEFT_BRACKET) {
		return {"Missing opening bracket", this->tokens.front()->getRange(), this->context};
	}
	delete this->tokens.front();
	this->tokens.pop_front();
	if (this->tokens.front()->getType() == TokenType::TOKEN_TYPE_STRUCT_NAME) {
		dynamic_cast<TypeToken *>(this->currentLine->back())
			->setListType(this->tokens.front()->getStringValue());
	} else if (this->tokens.front()->getType() == TokenType::TOKEN_TYPE_VALUE_TYPE) {
		ValueType type = stringToType(this->tokens.front()->getStringValue());
		if (type == LIST) {
			return {"List type cannot contain another list", this->tokens.front()->getRange(),
					this->context};
		}
		dynamic_cast<TypeToken *>(this->currentLine->back())->setListType(type);
	} else {
		return {"List type require a content type in the form 'list[type]'", token->getRange(),
				this->context};
	}
	delete this->tokens.front();
	this->tokens.pop_front();
	if (this->tokens.front()->getType() != TokenType::TOKEN_TYPE_RIGHT_BRACKET) {
		return {"Missing ']' after list type", this->tokens.front()->getRange(), this->context};
	}
	this->currentLine->back()->setRange(token->getRange().merge(this->tokens.front()->getRange()));
	delete this->tokens.front();
	this->tokens.pop_front();
	return {};
}

/**
 * @brief take a line of code and convert it to a vector of tokens
 *
 * @param lineNumber the current line number
 * @param lineString the current line of code
 * @param tokens the vector of tokens to fill
 * @return ExpressionResult if the line is a valid expression
 */
ExpressionResult Lexer::tokenize(int lineNumber, std::string_view lineString,
								 std::deque<Token *> &tokens, const ContextPtr &context) {
	unsigned int column = 0;
	std::string_view value;
	matchResult result;
	size_t i;
	size_t matchSize;
	bool match = false;
	while (!lineString.empty()) {
		i = 0;
		match = false;
		while (!match && i < tokenRegexes.size()) {
			Matcher const regex = std::get<0>(tokenRegexes[i]);
			if ((result = regex(lineString.data())).has_value()) {
				match = true;
				value = result.value().first;
				matchSize = result.value().second;

				TokenType const type = std::get<1>(tokenRegexes[i]);
				if (type == TokenType::TOKEN_TYPE_COMMENT) {
					return {};
				}
				if (type != TokenType::TOKEN_TYPE_INDENT) {
					tokens.push_back(new StringToken(lineNumber, column, type, value, matchSize));
				} else {
					column += 1;
				}
			}
			i++;
		}

		if (!match) {
			return {"Unexpected char : '" + std::string(1, lineString[0]) + "'",
					TextRange(lineNumber, column, 1), context};
		}

		column += matchSize;
		lineString = lineString.substr(matchSize, lineString.size() - 1);

		// remove spaces
		while (!lineString.empty() && lineString[0] == ' ') {
			lineString = lineString.substr(1, lineString.size() - 1);
			column++;
		}
	}

	while (!tokens.empty() && tokens.back()->getType() == TokenType::TOKEN_TYPE_END_OF_LINE) {
		delete tokens.back();
		tokens.pop_back();
	}

	return {};
}
