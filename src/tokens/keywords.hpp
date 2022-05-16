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
")");

#define BLOCK_OPERATORS 4
const std::string blockOpeners[BLOCK_OPERATORS] = {
	"fun",
	"if",
	"while",
	"for"
};

const std::string blockClosers[BLOCK_OPERATORS] = {
	"nuf",
	"fi",
	"elihw",
	"rof"
};