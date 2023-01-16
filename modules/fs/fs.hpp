#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include "expressionresult/expressionresult.hpp"
#include "cppmodule/cppmodule.hpp"
#include "value/types/struct.hpp"

namespace fs = std::filesystem;

ExpressionResult loader(CppModule *module);

ModuleAPI moduleAPI {
	"fs",
	"A module to manipulate files",
	"1.0",
	"robotechnic",
	loader
};
