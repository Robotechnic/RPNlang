#include "shell/colors.hpp"

/**
 * @brief convert a color name to a ANSII color code
 * 
 * @param color the color to print
 * @return std::string the ANSII escape sequence
 */
std::string toEscapeSequence(color color) {
	switch (color) {
		case BLACK:
			return "\033[30m";
		case RED:
			return "\033[31m";
		case GREEN:
			return "\033[32m";
		case YELLOW:
			return "\033[33m";
		case BLUE:
			return "\033[34m";
		case MAGENTA:
			return "\033[35m";
		case CYAN:
			return "\033[36m";
		case WHITE:
			return "\033[37m";
		case RESET:
			return "\033[0m";
		case DEFAULT:
			return "\033[39m";
		default:
			return "";
	};
}

/**
 * @brief conver a color name to a ANSII color code
 * 
 * @param background the text background to print
 * @return std::string the ANSII escape sequence
 */
std::string toEscapeSequence(background background) {
	switch (background) {
		case BG_BLACK:
			return "\033[40m";
		case BG_RED:
			return "\033[41m";
		case BG_GREEN:
			return "\033[42m";
		case BG_YELLOW:
			return "\033[43m";
		case BG_BLUE:
			return "\033[44m";
		case BG_MAGENTA:
			return "\033[45m";
		case BG_CYAN:
			return "\033[46m";
		case BG_WHITE:
			return "\033[47m";
		case BG_RESET:
			return "\033[0m";
		case BG_DEFAULT:
			return "\033[49m";
		default:
			return "";
	};
}

/**
 * @brief convert a format name to a ANSII format code
 * 
 * @param format the format to print
 * @return std::string the ANSII escape sequence
 */
std::string toEscapeSequence(format format) {
	switch (format) {
		case BOLD:
			return "\033[1m";
		case BOLD_RESET:
			return "\033[22m";
		case UNDERLINE:
			return "\033[4m";
		case UNDERLINE_RESET:
			return "\033[24m";
		case ITALIC:
			return "\033[3m";
		case ITALIC_RESET:
			return "\033[23m";
		case STRIKETHROUGH:
			return "\033[9m";
		case STRIKETHROUGH_RESET:
			return "\033[29m";
		case REVERSE:
			return "\033[7m";
		case REVERSE_RESET:
			return "\033[27m";
		case RESET_FORMAT:
			return "\033[0m";
		default:
			return "";
	};
}