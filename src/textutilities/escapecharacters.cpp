#include "textutilities/escapecharacters.hpp"

/**
 * @brief replace all ocurences of spetial character escape sequences with their corresponding character
 * 
 * @param str the string to replace the escape sequences in
 * @return std::string the string with the escape sequences replaced
 */
std::string escapeCharacters(std::string str) {
	for (int i = 0; i < ESCAPE_CHARACTERS; i++) {
		std::regex regex = escapes[i].first;
		std::string replacement = escapes[i].second;
		str = std::regex_replace(str, regex, replacement);
	}
	return str;
}

std::string removeEscapeCharacters(std::string const& str) {
	return std::regex_replace(str, std::regex("\n"), "\\n");
}