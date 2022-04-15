#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include "tokens/tokentypes.hpp"
#include "textrange/textrange.hpp"
#include "expressionresult/expressionresult.hpp"



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