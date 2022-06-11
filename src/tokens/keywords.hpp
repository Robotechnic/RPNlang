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
	"break"
")$");

#define BLOCK_OPENERS 4
const std::string blockOpeners[BLOCK_OPENERS] = {
	"fun",
	"if",
	"while",
	"for"
};

#define BLOCK_CLOSERS 4
const std::string blockClosers[BLOCK_CLOSERS] = {
	"nuf",
	"fi",
	"elihw",
	"rof"
};

#define BLOCK_INTERMEDIATES 1
const std::string blockIntermediates[BLOCK_INTERMEDIATES] = {
	"else"
};