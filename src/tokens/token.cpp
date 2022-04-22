#include "tokens/token.hpp"

Token::Token(int line, int column, TokenType type, std::string value) :
	value(value),
	type(type),
	line(line),
	column(column)
{}

Token::Token(const Token &other) : 
	value(other.value),
	type(other.type),
	line(other.line),
	column(other.column)
{}

ExpressionResult Token::tokenize(int line, std::string lineString, std::vector<Token> &tokens) {
	int column = 0;
	std::smatch match;
	while (lineString.size() > 0) {
		int i = 0;
		match = std::smatch();
		while (match.size() == 0 && i < TOKEN_TYPES) {
			std::regex regex = std::get<0>(tokenRegexes[i]);
			if (std::regex_search(lineString, match, regex)) {
				TokenType type = std::get<1>(tokenRegexes[i]);
				std::string value = match.str(1);
				tokens.push_back(Token(line, column, type, value));
			}
			i++;
		}
		
		if (match.size() == 0) {
			return ExpressionResult("Unexpected char", TextRange(line, column, 1));
		}

		column += match.str().size();
		lineString = lineString.substr(match.str().size(), lineString.size() - 1);
		
		while (lineString[0] == ' ') {
			lineString = lineString.substr(1, lineString.size() - 1);
			column++;
		}
	}
	return ExpressionResult();
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

int Token::getLine() const {
	return this->line;
}
int Token::getColumn() const {
	return this->column;
}

TextRange Token::getRange() const {
	return TextRange(this->line, this->column, this->value.size());
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