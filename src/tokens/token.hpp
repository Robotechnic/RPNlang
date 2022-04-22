#pragma once


#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <tuple>
#include "tokens/tokentypes.hpp"
#include "tokens/keywords.hpp"
#include "textutilities/textrange.hpp"
#include "expressionresult/expressionresult.hpp"

// values types
const std::regex floatRegex("^((?:[0-9]+)?\\.(?:[0-9]+)?)");
const std::regex intRegex("^([0-9]+)");
const std::regex boolRegex("^(true|false)");
const std::regex stringRegex("^(?:\")([^\"]*)(?:\")");
const std::regex typeRegex("^(int|float|bool|string)");

// control structures
const std::regex indentBlockRegex("^(\t)");
const std::regex defineTokenRegex("^(->)");
const std::regex controlEndRegex("^(:)");
const std::regex functionReferenceRegex("^(:[a-z][a-zA-Z]*)");

// variables
const std::regex affectTokenRegex("^(=)");
const std::regex literalRegex("^([a-z][a-zA-Z]*)");

// math operators
const std::regex operatorRegex("^([+-/*^])");

// multi lines statements
const std::regex lineSeparatorRegex("^(\\n|;)");

#define TOKEN_TYPES 13
/* order matter because some tokens can be substrings of others
 * exemple: an int can be a substring of a float
 * true, false and types can be keywords or literals
 */
const std::tuple<std::regex, TokenType> tokenRegexes[TOKEN_TYPES] = {
	std::make_tuple(floatRegex, TOKEN_TYPE_FLOAT),
	std::make_tuple(intRegex, TOKEN_TYPE_INT),
	std::make_tuple(boolRegex, TOKEN_TYPE_BOOL),
	std::make_tuple(stringRegex, TOKEN_TYPE_STRING),
	std::make_tuple(typeRegex, TOKEN_TYPE_VALUE_TYPE),

	std::make_tuple(indentBlockRegex, TOKEN_TYPE_INDENT),
	std::make_tuple(operatorRegex, TOKEN_TYPE_OPERATOR),
	std::make_tuple(functionReferenceRegex, TOKEN_TYPE_FUNCTION_REF),
	std::make_tuple(defineTokenRegex, TOKEN_TYPE_DEFINE),
	std::make_tuple(controlEndRegex,TOKEN_TYPE_CONTROL_END),

	std::make_tuple(lineSeparatorRegex, TOKEN_TYPE_END_OF_LINE),
	std::make_tuple(keywordsRegex, TOKEN_TYPE_KEYWORD),
	std::make_tuple(literalRegex, TOKEN_TYPE_LITERAL)
};

class Token {
	public:
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

		static ExpressionResult tokenize(int line, std::string lineString, std::vector<Token> &tokens);

	private:
		std::string value;
		TokenType type;
		int line;
		int column;
};

std::ostream &operator<<(std::ostream &os, const Token &token);
std::ostream &operator<<(std::ostream &os, const std::vector<Token> &tokens);