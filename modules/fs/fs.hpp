#pragma once

#include "cppmodule/cppmodule.hpp"
#include "expressionresult/expressionresult.hpp"
#include "value/types/struct.hpp"
#include <filesystem>
#include <fstream>
#include <memory>

namespace fs = std::filesystem;

ExpressionResult loader(CppModule *module);

ModuleAPI moduleAPI{"fs", "A module to manipulate files", "1.0", "robotechnic", loader};
