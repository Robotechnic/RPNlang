#pragma once

#include <cmath>
#include "expressionresult/expressionresult.hpp"
#include "cppmodule/cppmodule.hpp"
#include "value/value.hpp"
#include "value/types/numbers/float.hpp"

ExpressionResult loader(CppModule &module);

ModuleAPI moduleAPI {
	"Math",
	"RPNlang math module",
	"1.0",
	"Robotechnic",
	loader
};