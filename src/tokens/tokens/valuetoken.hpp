#pragma once

#include "tokens/token.hpp"
#include "value/value.hpp"

class ValueToken : public Token {
	public:
		ValueToken(Value *value);
		~ValueToken();

		Value *getValue();

		std::string getStringValue() const override;

	private:
		bool accessed;
		Value *value;
};