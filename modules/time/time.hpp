#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

#include "cppmodule/cppmodule.hpp"
#include "expressionresult/expressionresult.hpp"
#include "value/types/numbers/float.hpp"
#include "value/types/numbers/int.hpp"
#include "value/types/string.hpp"
#include "value/value.hpp"

ExpressionResult loader(CppModule *module);

ModuleAPI moduleAPI{"Time", "RPNlang time module", "1.0", "Robotechnic", loader};