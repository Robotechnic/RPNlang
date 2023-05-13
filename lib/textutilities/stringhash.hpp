#pragma once

#include <string>
#include <string_view>

class StringHash {
  public:
	std::size_t operator()(const std::string &str) const {
		return std::hash<std::string_view>()(str);
	}

	std::size_t operator()(const std::string_view &str) const {
		return std::hash<std::string_view>()(str);
	}

	std::size_t operator()(const char *str) const {
		return std::hash<std::string_view>()(str);
	}
};