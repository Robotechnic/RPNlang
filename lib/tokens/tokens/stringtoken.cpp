#include "tokens/tokens/stringtoken.hpp"

StringToken::StringToken(int line, int column, TokenType type, std::string_view value) : 
	Token(TextRange(line, column, value.size()), type), 
	value(value) {}

StringToken::StringToken(TextRange range, TokenType type, std::string_view value) :
	Token(range, type),
	value(value) {}


void StringToken::setValue(std::string_view value) {
	this->value = value;
}

std::string StringToken::getValue() const {
	return this->value;
}

std::string StringToken::getStringValue() const {
	return this->value;
}