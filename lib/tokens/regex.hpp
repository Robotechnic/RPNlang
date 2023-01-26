#pragma once

#include <string>
#include <array>
#include <functional>
#include <optional>
#include <tuple>
#include "ctre/ctre.hpp"
#include "tokens/tokentypes.hpp"

typedef std::optional<std::pair<std::string_view, size_t>> matchResult;
typedef std::function<matchResult(std::string_view)> Matcher;


matchResult floatMatch(std::string_view str);
matchResult intMatch(std::string_view str);
matchResult binNumMatch(std::string_view str);
matchResult hexNumMatch(std::string_view str);
matchResult boolMatch(std::string_view str);
matchResult stringMatch(std::string_view str);
matchResult fStringMatch(std::string_view str);
matchResult typeMatch(std::string_view str);
matchResult indentBlockMatch(std::string_view str);
matchResult colonMatch(std::string_view str);
matchResult arrowMatch(std::string_view str);
matchResult leftBracket(std::string_view str);
matchResult rightBracket(std::string_view str);
matchResult expressionSeparatorMatch(std::string_view str);
matchResult affectTokenMatch(std::string_view str);
matchResult dotMatch(std::string_view str);
matchResult capsLiteralMatch(std::string_view str);
matchResult literalMatch(std::string_view str);
matchResult structMatch(std::string_view str);
matchResult operatorMatch(std::string_view str);
matchResult booleanOperatorMatch(std::string_view str);
matchResult lineSeparatorMatch(std::string_view str);
matchResult commentMatch(std::string_view str);



/* order matter because some tokens can be substrings of others
 * exemple: an int can be a substring of a float
 * true, false and types can be keywords or literals
 */
const std::array<std::pair<Matcher, TokenType>, 23> tokenRegexes = {
	std::make_pair(binNumMatch,              TOKEN_TYPE_BIN),
	std::make_pair(hexNumMatch,              TOKEN_TYPE_HEX),
	std::make_pair(floatMatch,               TOKEN_TYPE_FLOAT),
	std::make_pair(intMatch,                 TOKEN_TYPE_INT),
	std::make_pair(arrowMatch,               TOKEN_TYPE_ARROW),
	std::make_pair(leftBracket,              TOKEN_TYPE_LEFT_BRACKET),
	std::make_pair(rightBracket,             TOKEN_TYPE_RIGHT_BRACKET),
	std::make_pair(boolMatch,                TOKEN_TYPE_BOOL),
	std::make_pair(stringMatch,              TOKEN_TYPE_STRING),
	std::make_pair(fStringMatch,             TOKEN_TYPE_FSTRING),
	std::make_pair(typeMatch,                TOKEN_TYPE_VALUE_TYPE),
	std::make_pair(indentBlockMatch,         TOKEN_TYPE_INDENT),
	std::make_pair(colonMatch,               TOKEN_TYPE_COLON),
	std::make_pair(booleanOperatorMatch,     TOKEN_TYPE_BOOLEAN_OPERATOR),
	std::make_pair(affectTokenMatch,         TOKEN_TYPE_ASSIGNMENT),
	std::make_pair(operatorMatch,            TOKEN_TYPE_OPERATOR),
	std::make_pair(expressionSeparatorMatch, TOKEN_TYPE_EXPRESSION_SEPARATOR),
	std::make_pair(lineSeparatorMatch,       TOKEN_TYPE_END_OF_LINE),
	std::make_pair(dotMatch,                 TOKEN_TYPE_DOT),
	std::make_pair(capsLiteralMatch,         TOKEN_TYPE_LITERAL),
	std::make_pair(structMatch,              TOKEN_TYPE_STRUCT_NAME),
	std::make_pair(literalMatch,             TOKEN_TYPE_LITERAL),
	std::make_pair(commentMatch,             TOKEN_TYPE_COMMENT)
};