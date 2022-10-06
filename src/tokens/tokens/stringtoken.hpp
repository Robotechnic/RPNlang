#pragma once

#include "tokens/token.hpp"
#include "tokens/tokentypes.hpp"

class StringToken : public Token {
	public: 
		StringToken(int line, int column, TokenType type, std::string value);
		~StringToken() override {};

		void setValue(std::string value);
		std::string getValue() const;
		std::string getStringValue() const;

	private:
		std::string value;
};