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

#define BLOCK_OPENERS 4
const std::string blockOpeners[BLOCK_OPENERS] = {
	"fun",
	"if",
	"while",
	"for"
};

#define BLOCK_CLOSERS 5
const std::string blockClosers[BLOCK_CLOSERS] = {
	"nuf",
	"fi",
	"elihw",
	"rof",
	"else" // else if a spetial case but in my language it closes the if block
};