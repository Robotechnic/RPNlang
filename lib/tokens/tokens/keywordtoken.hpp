#pragma once

#include <string_view>
#include "tokens/keywords.hpp"
#include "tokens/token.hpp"

class KeywordToken : public Token {
	public:
		KeywordToken(TextRange range, std::string_view keyword);

		KeywordEnum getKeyword() const;
		std::string getStringValue() const override;

		static std::string keywordString(KeywordEnum keyword);
	
	private:
		KeywordEnum keyword;
};