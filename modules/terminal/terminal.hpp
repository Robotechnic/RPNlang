#pragma once

#include <sys/ioctl.h>
#include <termios.h>
#include <string_view>

#include "expressionresult/expressionresult.hpp"
#include "cppmodule/cppmodule.hpp"
#include "value/types/numbers/int.hpp"
#include "value/types/string.hpp"

enum TEXT_COLORS {
	BLACK   = 30,
	RED     = 31,
	GREEN   = 32,
	YELLOW  = 33,
	BLUE    = 34,
	PURPLE  = 35,
	CYAN    = 36,
	WHITE   = 37,
	DEFAULT = 39,
	RESET   = 0
};

enum BACKGROUND_COLORS {
	BACKGROUND_BLACK   = 40,
	BACKGROUND_RED     = 41,
	BACKGROUND_GREEN   = 42,
	BACKGROUND_YELLOW  = 43,
	BACKGROUND_BLUE    = 44,
	BACKGROUND_PURPLE  = 45,
	BACKGROUND_CYAN    = 46,
	BACKGROUND_WHITE   = 47,
	BACKGROUND_DEFAULT = 49
};

ExpressionResult loader(CppModule *module);

ModuleAPI moduleAPI {
	"terminalMatrix",
	"Allow to draw what you whant on the terminal and manage user input",
	"1.0",
	"robotechnic",
	loader
};
