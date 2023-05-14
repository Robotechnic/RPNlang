#pragma once

#include <array>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <tuple>

std::string escapeCharacters(std::string_view str);
std::string removeEscapeCharacters(std::string_view str);