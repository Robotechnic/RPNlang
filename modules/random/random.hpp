#pragma once

#include <ctime>
#include <random>

#include "expressionresult/expressionresult.hpp"
#include "value/value.hpp"
#include "value/types/none.hpp"
#include "value/types/numbers/int.hpp"
#include "value/types/numbers/float.hpp"
#include "cppmodule/cppmodule.hpp"

ExpressionResult randomLoader(CppModule &module);