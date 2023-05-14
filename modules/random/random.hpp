#pragma once

#include <ctime>
#include <random>

#include "cppmodule/cppmodule.hpp"
#include "expressionresult/expressionresult.hpp"
#include "value/types/none.hpp"
#include "value/types/numbers/float.hpp"
#include "value/types/numbers/int.hpp"
#include "value/value.hpp"

ExpressionResult loader(CppModule *module);

ModuleAPI moduleAPI{"Random", "RPNlang random module", "1.0", "Robotechnic", loader};