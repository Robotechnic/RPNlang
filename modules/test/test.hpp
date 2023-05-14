#pragma once
#include "cppmodule/cppmodule.hpp"
#include "expressionresult/expressionresult.hpp"
#include "value/types/none.hpp"
#include "value/types/string.hpp"

ExpressionResult loader(CppModule *module);

ModuleAPI moduleAPI{"Test", "RPNlang test module", "1.0", "Robotechnic", loader};