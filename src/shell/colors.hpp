#pragma once

#include <string>

enum color {
	BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
	RESET,
	DEFAULT
};

enum background {
	BG_BLACK,
	BG_RED,
	BG_GREEN,
	BG_YELLOW,
	BG_BLUE,
	BG_MAGENTA,
	BG_CYAN,
	BG_WHITE,
	BG_RESET,
	BG_DEFAULT
};

enum format {
	BOLD,
	BOLD_RESET,
	UNDERLINE,
	UNDERLINE_RESET,
	ITALIC,
	ITALIC_RESET,
	STRIKETHROUGH,
	STRIKETHROUGH_RESET,
	REVERSE,
	REVERSE_RESET,
	RESET_FORMAT
};

std::string toEscapeSequence(color color);
std::string toEscapeSequence(background background);
std::string toEscapeSequence(format format);