#pragma once

enum TokenType {
	TOKEN_TYPE_INT,
	TOKEN_TYPE_HEX,
	TOKEN_TYPE_BIN,
	TOKEN_TYPE_FLOAT,
	TOKEN_TYPE_BOOL,
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_FSTRING,
	TOKEN_TYPE_OPERATOR,
	TOKEN_TYPE_BOOLEAN_OPERATOR,
	TOKEN_TYPE_LITERAL,
	TOKEN_TYPE_VALUE_TYPE,
	TOKEN_TYPE_INDENT,
	TOKEN_TYPE_COLON,
	TOKEN_TYPE_ARROW,
	TOKEN_TYPE_EXPRESSION_SEPARATOR,
	TOKEN_TYPE_END_OF_LINE,
	TOKEN_TYPE_KEYWORD,
	TOKEN_TYPE_AFFECT,
	TOKEN_TYPE_COMMENT,
	TOKEN_TYPE_UNKNOWN
};