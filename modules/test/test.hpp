#pragma once
#include "expressionresult/expressionresult.hpp"
#include "cppmodule/cppmodule.hpp"
#include "value/types/string.hpp"
#include "value/types/none.hpp"


ExpressionResult loader(CppModule *module);


ModuleAPI moduleAPI {
	"Test",
	"RPNlang test module",
	"1.0",
	"Robotechnic",
	loader
};