#include "textutilities/escapecharacters.hpp"

std::string escapeCharacters(std::string str) {
	for (int i = 0; i < ESCAPE_CHARACTERS; i++) {
		std::regex regex = escapes[i].first;
		std::string replacement = escapes[i].second;
		str = std::regex_replace(str, regex, replacement);
	}
	return str;
}