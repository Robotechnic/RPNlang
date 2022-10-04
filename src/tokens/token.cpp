#include "tokens/token.hpp"

Token::Token() :
	value(""),
	type(TOKEN_TYPE_UNKNOWN),
	line(0),
	column(0)
{
}

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

Token::~Token(){}

bool Token::isNumber() const{
	return this->type == TOKEN_TYPE_FLOAT || this->type == TOKEN_TYPE_INT;
}

void Token::setValue(std::string value) {
	this->value = value;
}

std::string Token::getValue() const {
	return this->value;
}

TokenType Token::getType() const {
	return this->type;
}

void Token::setType(TokenType type) {
	this->type = type;
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

/**
 * @brief convert a TokenType to its string representation
 * 
 * @param type the TokenType to convert
 * @return std::string the string representation of the TokenType
 */
std::string Token::stringType(TokenType type) {
	switch (type) {
		case TOKEN_TYPE_INT:
			return "int";
		case TOKEN_TYPE_HEX:
			return "hex number";
		case TOKEN_TYPE_BIN:
			return "binary number";
		case TOKEN_TYPE_FLOAT:
			return "float";
		case TOKEN_TYPE_STRING:
			return "string";
		case TOKEN_TYPE_BOOL:
			return "boolean";
		case TOKEN_TYPE_OPERATOR:
			return "math operator";
		case TOKEN_TYPE_LITERAL:
			return "literal";
		case TOKEN_TYPE_AFFECT:
			return "effectation";
		case TOKEN_TYPE_COLON:
			return "colon token";
		case TOKEN_TYPE_INDENT:
			return "indentation";
		case TOKEN_TYPE_VALUE_TYPE:
			return "value type";
		case TOKEN_TYPE_END_OF_LINE:
			return "end of line";
		case TOKEN_TYPE_BOOLEAN_OPERATOR:
			return "boolean operator";
		case TOKEN_TYPE_COMMENT:
			return "comment";
		case TOKEN_TYPE_FSTRING:
			return "fstring";
		case TOKEN_TYPE_EXPRESSION_SEPARATOR:
			return "expression separator";
		case TOKEN_TYPE_ARROW:
			return "arrow";
		case TOKEN_TYPE_PATH:
			return "path";
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

std::ostream &operator<<(std::ostream &os, std::queue<Token> tokens) {
	while (!tokens.empty()) {
		os << tokens.front() << " ";
		tokens.pop();
	}
	return os;
}