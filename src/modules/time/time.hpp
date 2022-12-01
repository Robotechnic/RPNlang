#pragma once

#include <chrono>
#include <ctime>
#include <thread>
#include <iostream>

#include "expressionresult/expressionresult.hpp"
#include "value/value.hpp"
#include "value/types/string.hpp"
#include "value/types/numbers/int.hpp"
#include "modules/builtinmodule.hpp"

ExpressionResult timeLoader(BuiltinModule &module);