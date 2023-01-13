#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <cstring>
#include <fstream>
#include <ostream>
#include <filesystem>

std::vector<std::string> split(std::string_view str, char delimiter);
std::string join(const std::vector<std::string> &str, char delimiter);
std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &v);
std::string extractFileName(std::string_view path);
std::string extractFilePath(std::string_view path);
bool openFile(std::ifstream &file, std::string_view fileName, std::string &error);