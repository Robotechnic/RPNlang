#pragma once

#include <ctime>
#include <random>

#include "expressionresult/expressionresult.hpp"
#include "cppmodule/cppmodule.hpp"
#include "value/value.hpp"
#include "value/types/none.hpp"
#include "value/types/numbers/int.hpp"
#include "value/types/numbers/float.hpp"

ExpressionResult loader(CppModule &module);

ModuleAPI moduleAPI {
	"Random",
	"RPNlang random module",
	"1.0",
	"Robotechnic",
	loader
};