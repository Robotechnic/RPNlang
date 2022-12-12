#pragma once

#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <regex>
#include <tuple>
#include <queue>
#include "tokens/tokentypes.hpp"
#include "tokens/regex.hpp"
#include "textutilities/textrange.hpp"
#include "textutilities/escapecharacters.hpp"

/* order matter because some tokens can be substrings of others
 * exemple: an int can be a substring of a float
 * true, false and types can be keywords or literals
 */
const std::array<std::pair<std::regex, TokenType>, 19> tokenRegexes = {
	std::make_pair(commentRegex, TOKEN_TYPE_COMMENT),
	std::make_pair(binNumRegex, TOKEN_TYPE_BIN),
	std::make_pair(hexNumRegex, TOKEN_TYPE_HEX),
	std::make_pair(arrowRegex, TOKEN_TYPE_ARROW),
	std::make_pair(floatRegex, TOKEN_TYPE_FLOAT),
	std::make_pair(intRegex, TOKEN_TYPE_INT),
	std::make_pair(boolRegex, TOKEN_TYPE_BOOL),
	std::make_pair(stringRegex, TOKEN_TYPE_STRING),
	std::make_pair(fStringRegex, TOKEN_TYPE_FSTRING),
	std::make_pair(typeRegex, TOKEN_TYPE_VALUE_TYPE),
	std::make_pair(indentBlockRegex, TOKEN_TYPE_INDENT),
	std::make_pair(colonRegex,TOKEN_TYPE_COLON),
	std::make_pair(booleanOperatorRegex, TOKEN_TYPE_BOOLEAN_OPERATOR),
	std::make_pair(affectTokenRegex, TOKEN_TYPE_ASSIGNMENT),
	std::make_pair(operatorRegex, TOKEN_TYPE_OPERATOR),
	std::make_pair(expressionSeparatorRegex, TOKEN_TYPE_EXPRESSION_SEPARATOR),
	std::make_pair(lineSeparatorRegex, TOKEN_TYPE_END_OF_LINE),
	std::make_pair(path, TOKEN_TYPE_PATH),
	std::make_pair(literalRegex, TOKEN_TYPE_LITERAL)
};

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
		inline static std::string stringType(TokenType type);

	protected:
		TextRange range;
		TokenType type;
};

std::ostream &operator<<(std::ostream &os, const Token *token);
std::ostream &operator<<(std::ostream &os, const std::vector<Token*> &tokens);
std::ostream &operator<<(std::ostream &os, std::queue<Token*> tokens);