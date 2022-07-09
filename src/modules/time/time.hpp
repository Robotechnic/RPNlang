#pragma once

#include <chrono>
#include <ctime>
#include <thread>
#include <iostream>

#include "expressionresult/expressionresult.hpp"
#include "value/valuetype.hpp"
#include "value/value.hpp"

ExpressionResult timeLoader(BuiltinModule &module);