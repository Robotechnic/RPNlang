#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <tuple>
#include <queue>
#include "tokens/tokentypes.hpp"
#include "tokens/regex.hpp"
#include "textutilities/textrange.hpp"
#include "textutilities/escapecharacters.hpp"

#define TOKEN_TYPES 19
/* order matter because some tokens can be substrings of others
 * exemple: an int can be a substring of a float
 * true, false and types can be keywords or literals
 */
const std::tuple<std::regex, TokenType> tokenRegexes[TOKEN_TYPES] = {
	std::make_tuple(commentRegex, TOKEN_TYPE_COMMENT),
	std::make_tuple(binNumRegex, TOKEN_TYPE_BIN),
	std::make_tuple(hexNumRegex, TOKEN_TYPE_HEX),
	std::make_tuple(arrowRegex, TOKEN_TYPE_ARROW),
	std::make_tuple(floatRegex, TOKEN_TYPE_FLOAT),
	std::make_tuple(intRegex, TOKEN_TYPE_INT),
	std::make_tuple(boolRegex, TOKEN_TYPE_BOOL),
	std::make_tuple(stringRegex, TOKEN_TYPE_STRING),
	std::make_tuple(fStringRegex, TOKEN_TYPE_FSTRING),
	std::make_tuple(typeRegex, TOKEN_TYPE_VALUE_TYPE),
	std::make_tuple(indentBlockRegex, TOKEN_TYPE_INDENT),
	std::make_tuple(colonRegex,TOKEN_TYPE_COLON),
	std::make_tuple(booleanOperatorRegex, TOKEN_TYPE_BOOLEAN_OPERATOR),
	std::make_tuple(affectTokenRegex, TOKEN_TYPE_AFFECT),
	std::make_tuple(operatorRegex, TOKEN_TYPE_OPERATOR),
	std::make_tuple(expressionSeparatorRegex, TOKEN_TYPE_EXPRESSION_SEPARATOR),
	std::make_tuple(lineSeparatorRegex, TOKEN_TYPE_END_OF_LINE),
	std::make_tuple(path, TOKEN_TYPE_PATH),
	std::make_tuple(literalRegex, TOKEN_TYPE_LITERAL)
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
		static std::string stringType(TokenType type);

	protected:
		TextRange range;
		TokenType type;
};

std::ostream &operator<<(std::ostream &os, const Token *token);
std::ostream &operator<<(std::ostream &os, const std::vector<Token*> &tokens);
std::ostream &operator<<(std::ostream &os, std::queue<Token*> tokens);