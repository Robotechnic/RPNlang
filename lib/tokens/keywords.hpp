#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "ctre/ctre.hpp"
#include "value/valuetypes.hpp"

constexpr auto keywordsRegex = ctre::match<"^("
	"fun|"
	"nuf|"
	"if|"
	"fi|"
	"else|"
	"while|"
	"elihw|"
	"for|"
	"rof|"
	"return|"
	"continue|"
	"break|"
	"try|"
	"catch|"
	"finally|"
	"yrt|"
	"struct|"
	"tcurts"
")$">;

enum KeywordEnum {
	KEYWORD_FUN,
	KEYWORD_NUF,
	KEYWORD_IF,
	KEYWORD_FI,
	KEYWORD_ELSE,
	KEYWORD_WHILE,
	KEYWORD_ELIHW,
	KEYWORD_FOR,
	KEYWORD_ROF,
	KEYWORD_RETURN,
	KEYWORD_CONTINUE,
	KEYWORD_BREAK,
	KEYWORD_TRY,
	KEYWORD_CATCH,
	KEYWORD_FINALLY,
	KEYWORD_YRT,
	KEYWORD_STRUCT,
	KEYWORD_TCURTS
};

const std::unordered_map<KeywordEnum, std::vector<KeywordEnum>> blockOpeners = {
	{KEYWORD_FUN    , {KEYWORD_NUF}},
	{KEYWORD_IF     , {KEYWORD_FI, KEYWORD_ELSE}},
	{KEYWORD_WHILE  , {KEYWORD_ELIHW}},
	{KEYWORD_FOR    , {KEYWORD_ROF}},
	{KEYWORD_TRY    , {KEYWORD_YRT, KEYWORD_CATCH, KEYWORD_FINALLY}},
	{KEYWORD_ELSE   , {KEYWORD_FI}},
	{KEYWORD_CATCH  , {KEYWORD_YRT, KEYWORD_FINALLY}},
	{KEYWORD_FINALLY, {KEYWORD_YRT}},
	{KEYWORD_STRUCT , {KEYWORD_TCURTS}}
};

const std::unordered_map<KeywordEnum, KeywordEnum> blockClosers = {
	{KEYWORD_NUF    , KEYWORD_FUN},
	{KEYWORD_FI     , KEYWORD_IF},
	{KEYWORD_ELIHW  , KEYWORD_WHILE},
	{KEYWORD_ROF    , KEYWORD_FOR},
	{KEYWORD_YRT    , KEYWORD_TRY},
	{KEYWORD_ELSE   , KEYWORD_IF},
	{KEYWORD_CATCH  , KEYWORD_TRY},
	{KEYWORD_FINALLY, KEYWORD_TRY},
	{KEYWORD_TCURTS , KEYWORD_STRUCT}
};

const std::unordered_map<KeywordEnum, std::vector<KeywordEnum>> parentDependency = {
	{KEYWORD_RETURN  , {KEYWORD_FUN}},
	{KEYWORD_CONTINUE, {KEYWORD_WHILE, KEYWORD_FOR}},
	{KEYWORD_BREAK   , {KEYWORD_WHILE, KEYWORD_FOR}}
};

const std::unordered_map<KeywordEnum, std::vector<RPNValueType>> linePatern = {
	{KEYWORD_FOR, {VARIABLE, INT, INT, INT}},
	{KEYWORD_WHILE, {BOOL}},
	{KEYWORD_IF, {BOOL}},
	{KEYWORD_TRY, {VARIABLE}}
};
