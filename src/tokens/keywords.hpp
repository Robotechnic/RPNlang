#pragma once

#include <string>
#include <regex>

const std::regex keywordsRegex("^("
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
")$");

#define BLOCK_OPENERS 5
const std::string blockOpeners[BLOCK_OPENERS] = {
	"fun",
	"if",
	"while",
	"for",
	"try"
};

#define BLOCK_CLOSERS 5
const std::string blockClosers[BLOCK_CLOSERS] = {
	"nuf",
	"fi",
	"elihw",
	"rof",
	"yrt"
};

#define BLOCK_INTERMEDIATES 3
const std::string blockIntermediates[BLOCK_INTERMEDIATES] = {
	"else",
	"catch",
	"finally"
};