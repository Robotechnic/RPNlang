#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <tuple>
#include <queue>
#include "tokens/tokentypes.hpp"
#include "tokens/keywords.hpp"
#include "textutilities/textrange.hpp"
#include "context/context.hpp"

class ExpressionResult;
#include "expressionresult/expressionresult.hpp"

// values types
const std::regex floatRegex("^((?:[-])?(?:[0-9]+)?\\.(?:[0-9]+)?)");
const std::regex intRegex("^((?:[-])?[0-9]+)");
const std::regex binNumRegex("^(?:0b)([01]+)");
const std::regex hexNumRegex("^(0x[0-9a-fA-F]+)");
const std::regex boolRegex("^(true|false)");
const std::regex stringRegex("^(?:\")([^\"]*)(?:\")");
const std::regex fStringRegex("^f(?:\")([^\"]*)(?:\")");
const std::regex typeRegex("^(int|float|bool|string|none)");

// control structures
const std::regex indentBlockRegex("^(\t)");
const std::regex colonRegex("^([:])");
const std::regex arrowRegex("^(->)");
const std::regex expressionSeparatorRegex("^(\\|)");

// variables
const std::regex affectTokenRegex("^(=)");
const std::regex literalRegex("^([a-z][a-zA-Z0-9]*|[A-Z0-9][A-Z0-9_]*)");

// math operators
const std::regex operatorRegex("^([+-/*^])");
const std::regex booleanOperatorRegex("^([<>]=?|==|!=)");

// multi lines statements
const std::regex lineSeparatorRegex("^(\\n)");

//comments
const std::regex commentRegex("^(#)");

#define TOKEN_TYPES 19
/* order matter because some tokens can be substrings of others
 * exemple: an int can be a substring of a float
 * true, false and types can be keywords or literals
 */
const std::tuple<std::regex, TokenType> tokenRegexes[TOKEN_TYPES] = {
	std::make_tuple(commentRegex, TOKEN_TYPE_COMMENT),
	std::make_tuple(binNumRegex, TOKEN_TYPE_BIN),
	std::make_tuple(hexNumRegex, TOKEN_TYPE_HEX),
	std::make_tuple(keywordsRegex, TOKEN_TYPE_KEYWORD),
	std::make_tuple(arrowRegex, TOKEN_TYPE_ARROW),
	std::make_tuple(floatRegex, TOKEN_TYPE_FLOAT),
	std::make_tuple(intRegex, TOKEN_TYPE_INT),
	std::make_tuple(boolRegex, TOKEN_TYPE_BOOL),
	std::make_tuple(stringRegex, TOKEN_TYPE_STRING),
	std::make_tuple(fStringRegex, TOKEN_TYPE_FSTRING),
	std::make_tuple(typeRegex, TOKEN_TYPE_VALUE_TYPE),
	std::make_tuple(indentBlockRegex, TOKEN_TYPE_INDENT),
	std::make_tuple(colonRegex,TOKEN_TYPE_COLON),
	std::make_tuple(booleanOperatorRegex, TOKEN_TYPE_BOOLEAN_OPERATOR),
	std::make_tuple(affectTokenRegex, TOKEN_TYPE_AFFECT),
	std::make_tuple(operatorRegex, TOKEN_TYPE_OPERATOR),
	std::make_tuple(expressionSeparatorRegex, TOKEN_TYPE_EXPRESSION_SEPARATOR),
	std::make_tuple(lineSeparatorRegex, TOKEN_TYPE_END_OF_LINE),
	std::make_tuple(literalRegex, TOKEN_TYPE_LITERAL)
};

class Token {
	public:
		Token();
		Token(int line, int column, TokenType type, std::string value);
		Token(const Token &other);

		std::string getValue() const;
		TokenType getType() const;
		std::string getStringType() const;

		int getLine() const;
		int getColumn() const;
		bool isNumber() const;
		TextRange getRange() const;

		static std::string stringType(TokenType type);

		static ExpressionResult tokenize(int line, std::string lineString, std::queue<Token> &tokens, const Context *context);

	private:
		std::string value;
		TokenType type;
		int line;
		int column;
};

std::ostream &operator<<(std::ostream &os, const Token &token);
std::ostream &operator<<(std::ostream &os, const std::vector<Token> &tokens);
std::ostream &operator<<(std::ostream &os, std::queue<Token> tokens);