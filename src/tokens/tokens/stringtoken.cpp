#include "tokens/tokens/stringtoken.hpp"

StringToken::StringToken(int line, int column, TokenType type, std::string value) : 
	Token(TextRange(line, column, value.size()), type), 
	value(value) {}

StringToken::StringToken(TextRange range, TokenType type, std::string value) :
	Token(range, type),
	value(value) {}


void StringToken::setValue(std::string value) {
	this->value = value;
}

std::string StringToken::getValue() const {
	return this->value;
}

std::string StringToken::getStringValue() const {
	return this->value;
}