#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <ostream>

std::vector<std::string> split(std::string str, char delimiter);
std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &v);