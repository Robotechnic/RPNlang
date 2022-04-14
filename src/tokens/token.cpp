#include "tokens/token.hpp"

Token::Token(int line, int column, TokenType type, std::string value) :
	value(value),
	type(type),
	line(line),
	column(column)
{
}

Token::Token(const Token &other) : 
	value(other.value),
	type(other.type),
	line(other.line),
	column(other.column)
{
}

bool Token::tokenize(int line, std::string lineString, std::vector<Token> &tokens, std::string &error) {
	std::stringstream ss(lineString);
	std::string token;
	int column = 0;
	while (ss >> token) {
		if (std::regex_match(token, intRegex)) {
			tokens.push_back(Token(line, column, TOKEN_TYPE_INT, token));
		} else if (std::regex_match(token, floatRegex)) {
			tokens.push_back(Token(line, column, TOKEN_TYPE_FLOAT, token));
		} else if (std::regex_match(token, stringRegex)) {
			token.pop_back();
			token.erase(0, 1);
			tokens.push_back(Token(line, column, TOKEN_TYPE_STRING, token));
		} else if (std::regex_match(token, operatorRegex)) {
			tokens.push_back(Token(line, column, TOKEN_TYPE_OPERATOR, token));
		} else if (std::regex_match(token, literalRegex)) {
			tokens.push_back(Token(line, column, TOKEN_TYPE_LITERAL, token));
		} else if (std::regex_match(token, affectToken)) {
			tokens.push_back(Token(line, column, TOKEN_TYPE_AFFECT, token));
		} else {
			error = "Invalid token: " + token;
			return false;
		}
		column += token.size() + 1;
	}
	return true;
}

bool Token::isNumber() const{
	return this->type == TOKEN_TYPE_FLOAT || this->type == TOKEN_TYPE_INT;
}

std::string Token::getValue() const {
	return this->value;
}

TokenType Token::getType() const {
	return this->type;
}

std::string Token::stringType(TokenType type) {
	switch (type) {
		case TOKEN_TYPE_INT:
			return "int";
		case TOKEN_TYPE_FLOAT:
			return "float";
		case TOKEN_TYPE_STRING:
			return "string";
		case TOKEN_TYPE_OPERATOR:
			return "operator";
		case TOKEN_TYPE_LITERAL:
			return "literal";
		case TOKEN_TYPE_AFFECT:
			return "effectation";
		default:
			return "unknown";
	}
}

std::string Token::getStringType() const {
	return Token::stringType(this->type);
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
	os << "Token(" << token.getValue() << ", " << token.getStringType() << ")";
	return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Token> &tokens) {
	for (const Token &token : tokens) {
		os << token << " ";
	}
	return os;
}