#pragma once

#include "tokens/token.hpp"
#include "tokens/tokentypes.hpp"

class StringToken : public Token {
	public: 
		StringToken(int line, int column, TokenType type, std::string_view value, size_t realSize = 0);
		StringToken(TextRange range, TokenType type, std::string_view value);
		~StringToken() override {};

		void setValue(std::string_view value);
		std::string getValue() const;
		std::string getStringValue() const;

	private:
		std::string value;
};