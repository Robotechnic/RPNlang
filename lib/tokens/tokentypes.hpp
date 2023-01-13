#pragma once

enum TokenType {
	// used only for the lexer they will be converted to other types directly
	TOKEN_TYPE_HEX,
	TOKEN_TYPE_BIN,
	
	// this tokens will be converted to a value token
	TOKEN_TYPE_INT,
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_FLOAT,
	TOKEN_TYPE_BOOL,
	TOKEN_TYPE_VALUE,
	TOKEN_TYPE_FSTRING,
	TOKEN_TYPE_LITERAL,
	TOKEN_TYPE_STRUCT_NAME,

	// this tokens are made with combination of others tokens by the lexer
	TOKEN_TYPE_FUNCTION_CALL,
	TOKEN_TYPE_MODULE_FUNCTION_CALL,
	TOKEN_TYPE_KEYWORD,
	TOKEN_TYPE_PATH,
	TOKEN_TYPE_STRUCT_ACCESS,

	// this tokens will stay as they are durring lexing
	TOKEN_TYPE_OPERATOR,
	TOKEN_TYPE_BOOLEAN_OPERATOR,
	TOKEN_TYPE_VALUE_TYPE,
	TOKEN_TYPE_ARROW,
	TOKEN_TYPE_COLON,
	TOKEN_TYPE_DOT,
	TOKEN_TYPE_EXPRESSION_SEPARATOR,
	TOKEN_TYPE_ASSIGNMENT,
	
	// this tokens are only used for formatting and will be removed
	TOKEN_TYPE_END_OF_LINE,
	TOKEN_TYPE_INDENT,
	TOKEN_TYPE_COMMENT,
	TOKEN_TYPE_UNKNOWN
};