#pragma once

#include <string>
#include <array>
#include <functional>
#include <optional>
#include <tuple>
#include "ctre/ctre.hpp"
#include "tokens/tokentypes.hpp"

using matchResult = std::optional<std::pair<std::string_view, size_t>>;
using Matcher = std::function<matchResult(std::string_view)>;

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
matchResult functionSignatureMatch(std::string_view str);
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
const std::array<std::pair<Matcher, TokenType>, 24> tokenRegexes = {
	std::make_pair(binNumMatch,              TokenType::TOKEN_TYPE_BIN),
	std::make_pair(hexNumMatch,              TokenType::TOKEN_TYPE_HEX),
	std::make_pair(floatMatch,               TokenType::TOKEN_TYPE_FLOAT),
	std::make_pair(intMatch,                 TokenType::TOKEN_TYPE_INT),
	std::make_pair(arrowMatch,               TokenType::TOKEN_TYPE_ARROW),
	std::make_pair(leftBracket,              TokenType::TOKEN_TYPE_LEFT_BRACKET),
	std::make_pair(rightBracket,             TokenType::TOKEN_TYPE_RIGHT_BRACKET),
	std::make_pair(boolMatch,                TokenType::TOKEN_TYPE_BOOL),
	std::make_pair(stringMatch,              TokenType::TOKEN_TYPE_STRING),
	std::make_pair(fStringMatch,             TokenType::TOKEN_TYPE_FSTRING),
	std::make_pair(typeMatch,                TokenType::TOKEN_TYPE_VALUE_TYPE),
	std::make_pair(indentBlockMatch,         TokenType::TOKEN_TYPE_INDENT),
	std::make_pair(colonMatch,               TokenType::TOKEN_TYPE_COLON),
	std::make_pair(booleanOperatorMatch,     TokenType::TOKEN_TYPE_BOOLEAN_OPERATOR),
	std::make_pair(affectTokenMatch,         TokenType::TOKEN_TYPE_ASSIGNMENT),
	std::make_pair(operatorMatch,            TokenType::TOKEN_TYPE_OPERATOR),
	std::make_pair(expressionSeparatorMatch, TokenType::TOKEN_TYPE_EXPRESSION_SEPARATOR),
	std::make_pair(lineSeparatorMatch,       TokenType::TOKEN_TYPE_END_OF_LINE),
	std::make_pair(dotMatch,                 TokenType::TOKEN_TYPE_DOT),
	std::make_pair(capsLiteralMatch,         TokenType::TOKEN_TYPE_LITERAL),
	std::make_pair(structMatch,              TokenType::TOKEN_TYPE_STRUCT_NAME),
	std::make_pair(functionSignatureMatch,   TokenType::TOKEN_TYPE_FUN_SIGNATURE),
	std::make_pair(literalMatch,             TokenType::TOKEN_TYPE_LITERAL),
	std::make_pair(commentMatch,             TokenType::TOKEN_TYPE_COMMENT)
};