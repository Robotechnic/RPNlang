#pragma once

#include <cstring>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

std::vector<std::string> split(std::string_view str, char delimiter);
std::string join(const std::vector<std::string> &str, char delimiter);
std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &v);
std::string extractFileName(std::string_view path);
std::string extractFilePath(std::string_view path);
bool openFile(std::ifstream &file, std::string_view fileName, std::string &error);