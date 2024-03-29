#pragma once

#include "ctre/ctre.hpp"
#include "value/valuetypes.hpp"
#include <string>
#include <unordered_map>
#include <vector>

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
										   "import|"
										   "importAs|"
										   "struct|"
										   "tcurts|"
										   "get|"
										   "funsig"
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
	KEYWORD_IMPORT,
	KEYWORD_IMPORTAS,
	KEYWORD_STRUCT,
	KEYWORD_TCURTS,
	KEYWORD_GET,
	KEYWORD_FUNSIG
};

const std::unordered_map<KeywordEnum, std::vector<KeywordEnum>> blockOpeners = {
	{KEYWORD_FUN, {KEYWORD_NUF}},
	{KEYWORD_IF, {KEYWORD_FI, KEYWORD_ELSE}},
	{KEYWORD_WHILE, {KEYWORD_ELIHW}},
	{KEYWORD_FOR, {KEYWORD_ROF}},
	{KEYWORD_TRY, {KEYWORD_YRT, KEYWORD_CATCH, KEYWORD_FINALLY}},
	{KEYWORD_ELSE, {KEYWORD_FI}},
	{KEYWORD_CATCH, {KEYWORD_YRT, KEYWORD_FINALLY}},
	{KEYWORD_FINALLY, {KEYWORD_YRT}},
	{KEYWORD_STRUCT, {KEYWORD_TCURTS}}};

const std::unordered_map<KeywordEnum, KeywordEnum> blockClosers = {
	{KEYWORD_NUF, KEYWORD_FUN},	  {KEYWORD_FI, KEYWORD_IF},		  {KEYWORD_ELIHW, KEYWORD_WHILE},
	{KEYWORD_ROF, KEYWORD_FOR},	  {KEYWORD_YRT, KEYWORD_TRY},	  {KEYWORD_ELSE, KEYWORD_IF},
	{KEYWORD_CATCH, KEYWORD_TRY}, {KEYWORD_FINALLY, KEYWORD_TRY}, {KEYWORD_TCURTS, KEYWORD_STRUCT}};

const std::unordered_map<KeywordEnum, std::vector<KeywordEnum>> parentDependency = {
	{KEYWORD_RETURN, {KEYWORD_FUN}},
	{KEYWORD_CONTINUE, {KEYWORD_WHILE, KEYWORD_FOR}},
	{KEYWORD_BREAK, {KEYWORD_WHILE, KEYWORD_FOR}},
	{KEYWORD_IMPORT, {}},
	{KEYWORD_IMPORTAS, {}},
	{KEYWORD_GET, {}},
	{KEYWORD_FUNSIG, {}}};

const std::unordered_map<KeywordEnum, std::vector<RPNValueType>> linePatern = {
	{KEYWORD_FOR, {VARIABLE, INT, INT, INT}},
	{KEYWORD_WHILE, {BOOL}},
	{KEYWORD_IF, {BOOL}},
	{KEYWORD_TRY, {VARIABLE}},
	{KEYWORD_IMPORT, {STRING}},
	{KEYWORD_IMPORTAS, {STRING, STRING}},
	{KEYWORD_GET, {{LIST, ANY}, INT}}};
