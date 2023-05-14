#include "tokens/token.hpp"

Token::Token() : type(TokenType::TOKEN_TYPE_UNKNOWN) {}
Token::Token(TokenType type) : type(type) {}
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

void Token::setRange(TextRange range) {
	this->range = range;
}

/**
 * @brief convert a TokenType to its string representation
 *
 * @param type the TokenType to convert
 * @return std::string_view the string representation of the TokenType
 */
std::string Token::stringType(TokenType type) {
	switch (type) {
		case TokenType::TOKEN_TYPE_INT:
			return "int";
		case TokenType::TOKEN_TYPE_HEX:
			return "hex number";
		case TokenType::TOKEN_TYPE_BIN:
			return "binary number";
		case TokenType::TOKEN_TYPE_FLOAT:
			return "float";
		case TokenType::TOKEN_TYPE_STRING:
			return "string";
		case TokenType::TOKEN_TYPE_BOOL:
			return "boolean";
		case TokenType::TOKEN_TYPE_OPERATOR:
			return "math operator";
		case TokenType::TOKEN_TYPE_LITERAL:
			return "literal";
		case TokenType::TOKEN_TYPE_ASSIGNMENT:
			return "assignment";
		case TokenType::TOKEN_TYPE_COLON:
			return "colon token";
		case TokenType::TOKEN_TYPE_INDENT:
			return "indentation";
		case TokenType::TOKEN_TYPE_VALUE_TYPE:
			return "value type";
		case TokenType::TOKEN_TYPE_END_OF_LINE:
			return "end of line";
		case TokenType::TOKEN_TYPE_BOOLEAN_OPERATOR:
			return "boolean operator";
		case TokenType::TOKEN_TYPE_COMMENT:
			return "comment";
		case TokenType::TOKEN_TYPE_FSTRING:
			return "fstring";
		case TokenType::TOKEN_TYPE_EXPRESSION_SEPARATOR:
			return "expression separator";
		case TokenType::TOKEN_TYPE_ARROW:
			return "arrow";
		case TokenType::TOKEN_TYPE_PATH:
			return "path";
		case TokenType::TOKEN_TYPE_DOT:
			return "dot";
		case TokenType::TOKEN_TYPE_VALUE:
			return "value";
		case TokenType::TOKEN_TYPE_FUNCTION_CALL:
			return "function call";
		case TokenType::TOKEN_TYPE_MODULE_FUNCTION_CALL:
			return "module function call";
		case TokenType::TOKEN_TYPE_STRUCT_NAME:
			return "struct name";
		case TokenType::TOKEN_TYPE_STRUCT_ACCESS:
			return "struct access";
		case TokenType::TOKEN_TYPE_KEYWORD:
			return "keyword";
		case TokenType::TOKEN_TYPE_LEFT_BRACKET:
			return "left bracket";
		case TokenType::TOKEN_TYPE_RIGHT_BRACKET:
			return "right bracket";
		case TokenType::TOKEN_TYPE_LIST_TYPE:
			return "list type";
		case TokenType::TOKEN_TYPE_FUN_SIGNATURE:
			return "function signature";
		default:
			return "unknown";
	}
}

std::string Token::getStringType() const {
	return Token::stringType(this->type);
}

std::ostream &operator<<(std::ostream &os, const Token *token) {
	os << "Token(" << escapeCharacters(token->getStringValue()) << ", " << token->getStringType()
	   << ")";
	return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Token *> &tokens) {
	for (const Token *token : tokens) {
		os << token << " ";
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, std::deque<Token *> tokens) {
	while (!tokens.empty()) {
		os << tokens.front() << " ";
		tokens.pop_front();
	}
	return os;
}