#pragma once

#include <variant>
#include <string>

enum ValueType {
	INT,
	FLOAT,
	STRING,
	VARIABLE,
	FUNCTION,
	BOOL,
	PATH,
	BUILTIN_PATH,
	STRUCT_ACCESS,
	LIST,
	STRUCT,
	ANY,
	NONE
};

typedef std::variant<std::string, ValueType> RPNValueType;