#pragma once

#include <string>
#include <tuple>
#include <array>
#include <regex>
#include <iostream>
#include <string_view>

std::string escapeCharacters(std::string_view str);
std::string removeEscapeCharacters(std::string_view str);