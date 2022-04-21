#pragma once


#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <tuple>
#include "tokens/tokentypes.hpp"
#include "textutilities/textrange.hpp"
#include "expressionresult/expressionresult.hpp"


const std::regex intRegex("^([0-9]+)");
const std::regex floatRegex("^((?:[0-9]+)?\\.(?:[0-9]+)?)");
const std::regex boolRegex("^(true|false)");
const std::regex stringRegex("^(?:\")([^\"]*)(?:\")");
const std::regex operatorRegex("^([+-/*^])");
const std::regex literalRegex("^([a-zA-Z]+)");
const std::regex affectToken("^(=)");
const std::regex defineToken("^(->)");

#define TOKEN_TYPES 8
const std::tuple<std::regex, TokenType> tokenRegexes[TOKEN_TYPES] = {
	std::make_tuple(intRegex, TOKEN_TYPE_INT),
	std::make_tuple(floatRegex, TOKEN_TYPE_FLOAT),
	std::make_tuple(boolRegex, TOKEN_TYPE_BOOL),
	std::make_tuple(stringRegex, TOKEN_TYPE_STRING),
	std::make_tuple(operatorRegex, TOKEN_TYPE_OPERATOR),
	std::make_tuple(literalRegex, TOKEN_TYPE_LITERAL),
	std::make_tuple(affectToken, TOKEN_TYPE_AFFECT),
	std::make_tuple(defineToken, TOKEN_TYPE_DEFINE)
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