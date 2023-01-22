#pragma once

#include "tokens/token.hpp"
#include "value/value.hpp"

class ValueToken : public Token {
	public:
		ValueToken(Value *value, TokenType type = TOKEN_TYPE_VALUE);
		~ValueToken();

		Value *getValue() const;
		Value *&getValue();
		ValueType getValueType() const;

		inline std::string getStringValue() const override;

	private:
		bool accessed;
		Value *value;
};