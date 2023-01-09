#include "tokens/token.hpp"

Token::Token() : type(TOKEN_TYPE_UNKNOWN) {}
Token::Token(TokenType type) :	type(type) {}
Token::Token(const TextRange range, TokenType type) : range(range), type(type) {}

TokenType Token::getType() const {
	return this->type;
}

void Token::setType(TokenType type) {
	this->type = type;
}

TextRange Token::getRange() const {
	return this->range;
}

/**
 * @brief convert a TokenType to its string representation
 * 
 * @param type the TokenType to convert
 * @return std::string_view the string representation of the TokenType
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
		case TOKEN_TYPE_ASSIGNMENT:
			return "assignment";
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
		case TOKEN_TYPE_DOT:
			return "dot";
		case TOKEN_TYPE_VALUE:
			return "value";
		case TOKEN_TYPE_FUNCTION_CALL:
			return "function call";
		case TOKEN_TYPE_MODULE_FUNCTION_CALL:
			return "module function call";
		default:
			return "unknown";
	}
}

std::string Token::getStringType() const {
	return Token::stringType(this->type);
}


std::ostream &operator<<(std::ostream &os, const Token *token) {
	os << "Token(" << token->getStringValue() << ", " << token->getStringType() << ")";
	return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Token*> &tokens) {
	for (const Token *token : tokens) {
		os << token << " ";
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, std::queue<Token*> tokens) {
	while (!tokens.empty()) {
		os << tokens.front() << " ";
		tokens.pop();
	}
	return os;
}