#pragma once

#include "textutilities/escapecharacters.hpp"
#include "textutilities/textrange.hpp"
#include "tokens/regex.hpp"
#include "tokens/tokentypes.hpp"
#include <queue>
#include <span>
#include <string>
#include <string_view>
#include <vector>

class Token {
  public:
	Token();
	Token(TokenType type);
	Token(TextRange range, TokenType type);
	virtual ~Token() = default;

	TokenType getType() const;
	void setType(TokenType type);
	std::string getStringType() const;

	virtual std::string getStringValue() const = 0;

	TextRange getRange() const;
	void setRange(TextRange range);
	static std::string stringType(TokenType type);

  protected:
	TextRange range;
	TokenType type;
};

std::ostream &operator<<(std::ostream &os, const Token *token);
std::ostream &operator<<(std::ostream &os, const std::vector<Token *> &tokens);
std::ostream &operator<<(std::ostream &os, std::deque<Token *> tokens);
