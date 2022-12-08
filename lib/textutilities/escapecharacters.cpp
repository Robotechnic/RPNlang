#include "textutilities/escapecharacters.hpp"


std::array<std::pair<std::regex, std::string>, 5> escapes = {
	std::make_pair(std::regex("\\\\\\\\"), "\\"),
	std::make_pair(std::regex("\\\\n"), "\n"),
	std::make_pair(std::regex("\\\\t"), "\t"),
	std::make_pair(std::regex("\\\\r"), "\r"),
	// ANSI color escape codes
	std::make_pair(std::regex("\\\\x1b\\[([0-9A-Za-z;]*)"), "\x1b[$1")
};

/**
 * @brief replace all ocurences of spetial character escape sequences with their corresponding character
 * 
 * @param str the string to replace the escape sequences in
 * @return std::string the string with the escape sequences replaced
 */
std::string escapeCharacters(std::string_view str) {
	std::string result = str.data();
	for (size_t i = 0; i < escapes.size(); i++) {
		std::regex regex = escapes[i].first;
		std::string replacement = escapes[i].second;
		result = std::regex_replace(result, regex, replacement);
	}
	return result;
}

std::string removeEscapeCharacters(std::string_view str) {
	return std::regex_replace(str.data(), std::regex("\n"), "\\n");
}