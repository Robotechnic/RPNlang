#pragma once

#include <string>
#include <regex>
#include <tuple>

#define ESCAPE_CHARACTERS 5
const std::pair<std::regex, std::string> escapes[] = {
	std::make_pair(std::regex("\\\\\\\\"), "\\"),
	std::make_pair(std::regex("\\\\n"), "\n"),
	std::make_pair(std::regex("\\\\t"), "\t"),
	std::make_pair(std::regex("\\\\r"), "\r"),
	// ANSI color escape codes
	std::make_pair(std::regex("\\\\x1b\\[([0-9A-Za-z;]*)"), "\x1b[$1")
};

std::string escapeCharacters(std::string str);