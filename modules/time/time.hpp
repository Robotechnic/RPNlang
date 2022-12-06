#pragma once

#include <chrono>
#include <ctime>
#include <thread>
#include <iostream>

#include "expressionresult/expressionresult.hpp"
#include "value/value.hpp"
#include "value/types/string.hpp"
#include "value/types/numbers/int.hpp"
#include "cppmodule/cppmodule.hpp"

ExpressionResult loader(CppModule &module);

ModuleAPI moduleAPI {
	"Time",
	"RPNlang time module",
	"1.0",
	"Robotechnic",
	loader
};