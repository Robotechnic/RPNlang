#pragma once

#include "tokens/token.hpp"
#include "value/value.hpp"

class ValueToken : public Token {
	public:
		ValueToken(Value *value, TokenType type = TokenType::TOKEN_TYPE_VALUE);
		~ValueToken() override;

		Value *getValue() const;
		Value *&getValue();
		ValueType getValueType() const;

		std::string getStringValue() const override;

	private:
		Value *value;
};