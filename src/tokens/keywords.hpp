#include <string>
#include <regex>

const std::regex keywordsRegex("^("
	"def|"
	"if|"
	"else|"
	"while|"
	"for|"
	"return"
")");