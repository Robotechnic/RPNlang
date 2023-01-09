#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <queue>
#include "tokens/tokentypes.hpp"
#include "tokens/regex.hpp"
#include "textutilities/textrange.hpp"
#include "textutilities/escapecharacters.hpp"


class Token {
	public:
		Token();
		Token(TokenType type);
		Token(TextRange range, TokenType type);
		virtual ~Token(){};
		
		TokenType getType() const;
		void setType(TokenType type);
		std::string getStringType() const;

		virtual std::string getStringValue() const = 0;

		bool isNumber() const;
		TextRange getRange() const;
		static std::string stringType(TokenType type);

	protected:
		TextRange range;
		TokenType type;
};

std::ostream &operator<<(std::ostream &os, const Token *token);
std::ostream &operator<<(std::ostream &os, const std::vector<Token*> &tokens);
std::ostream &operator<<(std::ostream &os, std::queue<Token*> tokens);