#pragma once

#include "cppmodule/cppmodule.hpp"
#include "expressionresult/expressionresult.hpp"
#include "value/types/numbers/float.hpp"
#include "value/types/numbers/int.hpp"
#include "value/value.hpp"
#include <cmath>

ExpressionResult loader(CppModule *module);

ModuleAPI moduleAPI{"Math", "RPNlang math module", "1.0", "Robotechnic", loader};