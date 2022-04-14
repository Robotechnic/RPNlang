#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <regex>

enum TokenType {
	TOKEN_TYPE_INT,
	TOKEN_TYPE_FLOAT,
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_OPERATOR,
	TOKEN_TYPE_LITERAL,
	TOKEN_TYPE_AFFECT
};

const std::regex intRegex("^[0-9]+");
const std::regex floatRegex("^([0-9]+)?(\\.[0-9]+)?");
const std::regex stringRegex("^\".*\"");
const std::regex operatorRegex("^[+-/*]");
const std::regex literalRegex("^[a-zA-Z]+");
const std::regex affectToken("^=");

class Token {
	public:
		Token(int line, int column, TokenType type, std::string value);
		Token(const Token &other);

		std::string getValue() const;
		TokenType getType() const;
		std::string getStringType() const;

		bool isNumber() const;

		static std::string stringType(TokenType type);

		static bool tokenize(int line, std::string lineString, std::vector<Token> &tokens, std::string &error);

	private:
		std::string value;
		TokenType type;
		int line;
		int column;
};

std::ostream &operator<<(std::ostream &os, const Token &token);
std::ostream &operator<<(std::ostream &os, const std::vector<Token> &tokens);