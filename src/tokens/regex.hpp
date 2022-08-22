#pragma once

#include <string>
#include <regex>

// values types
const std::regex floatRegex("^((?:[-])?(?:[0-9]+)?\\.(?:[0-9]+)?)");
const std::regex intRegex("^((?:[-])?[0-9]+)");
const std::regex binNumRegex("^(?:0b)([01]+)");
const std::regex hexNumRegex("^(0x[0-9a-fA-F]+)");
const std::regex boolRegex("^(true|false)");
const std::regex stringRegex("^(?:\")([^\"]*)(?:\")");
const std::regex fStringRegex("^f(?:\")([^\"]*)(?:\")");
const std::regex typeRegex("^(int|float|bool|string|none)");

// control structures
const std::regex indentBlockRegex("^(\t)");
const std::regex colonRegex("^([:])");
const std::regex arrowRegex("^(->)");
const std::regex expressionSeparatorRegex("^(\\|)");

// variables
const std::regex affectTokenRegex("^(=)");

const std::string literal = "([a-z][a-zA-Z0-9]*|[A-Z0-9][A-Z0-9_]*)";
const std::regex path("^("+literal+"(?:\\."+literal+")+)");
const std::regex literalRegex("^" + literal);

// math operators
const std::regex operatorRegex("^([+-/*^%])");
const std::regex booleanOperatorRegex("^([<>]=?|==|!=)");

// multi lines statements
const std::regex lineSeparatorRegex("^(\\n)");

//comments
const std::regex commentRegex("^(#)");