#include "tokens/tokens/operatortoken.hpp"

OperatorToken::OperatorToken(TextRange range, TokenType type, std::string_view value) : 
	Token(range, type) {
	if (value == "+")
		this->type = OP_ADD;
	else if (value == "-")
		this->type = OP_SUB;
	else if (value == "*")
		this->type = OP_MUL;
	else if (value == "/")
		this->type = OP_DIV;
	else if (value == "^")
		this->type = OP_POW;
	else if (value == "%")
		this->type = OP_MOD;
	else if (value == "==")
		this->type = OP_EQ;
	else if (value == "!=")
		this->type = OP_NE;
	else if (value == ">")
		this->type = OP_GT;
	else if (value == ">=")
		this->type = OP_GE;
	else if (value == "<")
		this->type = OP_LT;
	else if (value == "<=")
		this->type = OP_LE;
	else
		throw std::runtime_error("Invalid operator token: " + std::string(value));
}

std::string OperatorToken::getStringValue() const {
	switch (this->type) {
		case OP_ADD:
			return "+";
		case OP_SUB:
			return "-";
		case OP_MUL:
			return "*";
		case OP_DIV:
			return "/";
		case OP_POW:
			return "^";
		case OP_MOD:
			return "%";
		case OP_EQ:
			return "==";
		case OP_NE:
			return "!=";
		case OP_GT:
			return ">";
		case OP_GE:
			return ">=";
		case OP_LT:
			return "<";
		case OP_LE:
			return "<=";
	}
	throw std::runtime_error("Invalid operator token");
}

OperatorToken::OperatorTypes OperatorToken::getOperatorType() const {
	return this->type;
}