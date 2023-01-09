#pragma once

#include "tokens/token.hpp"
#include "tokens/tokentypes.hpp"

class OperatorToken : public Token {
	public:
		OperatorToken(TextRange range, TokenType type, std::string_view value);

		std::string getStringValue() const override;

		enum OperatorTypes {
			OP_ADD,
			OP_SUB,
			OP_MUL,
			OP_DIV,
			OP_POW,
			OP_MOD,
			OP_EQ,
			OP_NE,
			OP_GT,
			OP_GE,
			OP_LT,
			OP_LE
		};

		OperatorTypes getOperatorType() const;

	private:
		OperatorTypes type;
};