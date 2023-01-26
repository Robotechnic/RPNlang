#include "terminal.hpp"

struct termios holdTermios;

ExpressionResult loader(CppModule *module) {
	ExpressionResult result;
	
	module->addFunction(
		"width", {}, INT, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			struct winsize w;
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			return new Int(w.ws_col, range, Value::INTERPRETER);
		}
	);

	module->addFunction(
		"height", {}, INT, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			struct winsize w;
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			return new Int(w.ws_row, range, Value::INTERPRETER);
		}
	);

	module->addFunction(
		"clear", {}, NONE, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			// reset color and clear screen
			std::cout << "\033[0m";
			std::cout << "\033[H";
			std::cout << "\033[0J";
			
			return None::empty();
		}
	);

	module->addFunction(
		"home", {}, NONE, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			// set cursor to top left of screen and reset colors
			std::cout << "\033[H";
			std::cout << "\033[0m";
			return None::empty();
		}
	);

	module->addFunction(
		"flush", {}, NONE, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			// flush output
			std::cout << std::flush;
			return None::empty();
		}
	);

	module->addFunction(
		"setPixelColor", {{"x", INT}, {"y", INT}, {"color", INT}}, NONE, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			int x = static_cast<Int*>(args[0])->getValue();
			int y = static_cast<Int*>(args[1])->getValue();
			int pixel = static_cast<Int*>(args[2])->getValue();
			if (pixel < 40 || pixel > 49) {
				return ExpressionResult(
					"Pixel color must be between 40 and 49",
					args[2]->getRange(),
					context
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[" << pixel << "m ";
			return None::empty();
		}
	);

	module->addFunction(
		"setRGBPixelColor", {{"x", INT}, {"y", INT}, {"red", INT}, {"blue", INT}, {"green", INT}}, NONE, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			int red = static_cast<Int*>(args[2])->getValue();
			int blue = static_cast<Int*>(args[3])->getValue();
			int green = static_cast<Int*>(args[4])->getValue();
			int x = static_cast<Int*>(args[0])->getValue();
			int y = static_cast<Int*>(args[1])->getValue();
			if (red < 0 || red > 255 || blue < 0 || blue > 255 || green < 0 || green > 255) {
				return ExpressionResult(
					"Pixel color must be between 0 and 255",
					red < 0 || red > 255 ? args[0]->getRange() : (blue < 0 || blue > 255 ? args[1]->getRange() : args[2]->getRange()),
					context
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[48;2;" << red << ";" << green << ";" << blue << "m ";
			return None::empty();
		}
	);

	module->addFunction(
		"setAscii", {{"x", INT}, {"y", INT}, {"letter", STRING}, {"color", INT}, {"background", INT}}, NONE, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			const std::string letter = args[2]->getStringValue();
			int x = static_cast<Int*>(args[0])->getValue();
			int y = static_cast<Int*>(args[1])->getValue();
			int color = static_cast<Int*>(args[3])->getValue();
			int background = static_cast<Int*>(args[4])->getValue();
			if (color < 30 || color > 39) {
				return ExpressionResult(
					"Color must be between 30 and 39",
					args[3]->getRange(),
					context
				);
			}
			if (background < 40 || background > 49) {
				return ExpressionResult(
					"Background must be between 40 and 49",
					args[4]->getRange(),
					context
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[" << color << ";" << background << "m" << letter;
			return None::empty();
		}
	);

	module->addFunction(
		"setRGBAscii", {{"x", INT}, {"y", INT},  {"letter", INT}, {"red", INT}, {"blue", INT}, {"green", INT}, {"backgroundRed", INT}, {"backgroundBlue", INT}, {"backgroundGreen", INT}}, NONE, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			int letter = static_cast<Int*>(args[2])->getValue();
			int x = static_cast<Int*>(args[0])->getValue();
			int y = static_cast<Int*>(args[1])->getValue();
			int red = static_cast<Int*>(args[3])->getValue();
			int blue = static_cast<Int*>(args[4])->getValue();
			int green = static_cast<Int*>(args[5])->getValue();
			int backgroundRed = static_cast<Int*>(args[6])->getValue();
			int backgroundBlue = static_cast<Int*>(args[7])->getValue();
			int backgroundGreen = static_cast<Int*>(args[8])->getValue();
			if (red < 0 || red > 255 || blue < 0 || blue > 255 || green < 0 || green > 255) {
				return ExpressionResult(
					"Pixel color must be between 0 and 255",
					red < 0 || red > 255 ? args[0]->getRange() : (blue < 0 || blue > 255 ? args[1]->getRange() : args[2]->getRange()),
					context
				);
			}
			if (backgroundRed < 0 || backgroundRed > 255 || backgroundBlue < 0 || backgroundBlue > 255 || backgroundGreen < 0 || backgroundGreen > 255) {
				return ExpressionResult(
					"Pixel color must be between 0 and 255",
					backgroundRed < 0 || backgroundRed > 255 ? args[3]->getRange() : (backgroundBlue < 0 || backgroundBlue > 255 ? args[4]->getRange() : args[5]->getRange()),
					context
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[38;2;" << red << ";" << green << ";" << blue << "m";
			std::cout << "\033[48;2;" << backgroundRed << ";" << backgroundGreen << ";" << backgroundBlue << "m" << char(letter);
			return None::empty();
		}
	);

	module->addFunction(
		"drawRect", {{"x", INT}, {"y", INT}, {"width", INT}, {"height", INT}, {"color", INT}}, NONE, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			int x = static_cast<Int*>(args[0])->getValue();
			int y = static_cast<Int*>(args[1])->getValue();
			int width = static_cast<Int*>(args[2])->getValue();
			int height = static_cast<Int*>(args[3])->getValue();
			int color = static_cast<Int*>(args[4])->getValue();
			if (color < 40 || color > 49) {
				return ExpressionResult(
					"Color must be between 40 and 49",
					args[4]->getRange(),
					context
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[" << color << "m";
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					std::cout << " ";
				}
				std::cout << "\033[" << y + i + 1 << ";" << x << "H";
			}
			return None::empty();
		}
	);

	module->addFunction(
		"drawText", {{"x", INT}, {"y", INT}, {"text", STRING}, {"color", INT}, {"background", INT}}, NONE, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			const std::string text = args[2]->getStringValue();
			int x = static_cast<Int*>(args[0])->getValue();
			int y = static_cast<Int*>(args[1])->getValue();
			int color = static_cast<Int*>(args[3])->getValue();
			int background = static_cast<Int*>(args[4])->getValue();
			if (color < 30 || color > 39) {
				return ExpressionResult(
					"Color must be between 30 and 39",
					args[3]->getRange(),
					context
				);
			}
			if (background < 40 || background > 49) {
				return ExpressionResult(
					"Background must be between 40 and 49",
					args[4]->getRange(),
					context
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[" << color << ";" << background << "m" << text;
			return None::empty();
		}
	);

	module->addFunction(
		"drawTextRGB", {{"x", INT}, {"y", INT},  {"text", STRING}, {"red", INT}, {"blue", INT}, {"green", INT}, {"backgroundRed", INT}, {"backgroundBlue", INT}, {"backgroundGreen", INT}}, NONE, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			const std::string text = args[0]->getStringValue();
			int x = static_cast<Int*>(args[1])->getValue();
			int y = static_cast<Int*>(args[2])->getValue();
			int red = static_cast<Int*>(args[3])->getValue();
			int blue = static_cast<Int*>(args[4])->getValue();
			int green = static_cast<Int*>(args[5])->getValue();
			int backgroundRed = static_cast<Int*>(args[6])->getValue();
			int backgroundBlue = static_cast<Int*>(args[7])->getValue();
			int backgroundGreen = static_cast<Int*>(args[8])->getValue();
			if (red < 0 || red > 255 || blue < 0 || blue > 255 || green < 0 || green > 255) {
				return ExpressionResult(
					"Pixel color must be between 0 and 255",
					red < 0 || red > 255 ? args[0]->getRange() : (blue < 0 || blue > 255 ? args[1]->getRange() : args[2]->getRange()),
					context
				);
			}
			if (backgroundRed < 0 || backgroundRed > 255 || backgroundBlue < 0 || backgroundBlue > 255 || backgroundGreen < 0 || backgroundGreen > 255) {
				return ExpressionResult(
					"Pixel color must be between 0 and 255",
					backgroundRed < 0 || backgroundRed > 255 ? args[3]->getRange() : (backgroundBlue < 0 || backgroundBlue > 255 ? args[4]->getRange() : args[5]->getRange()),
					context
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[38;2;" << red << ";" << green << ";" << blue << "m";
			std::cout << "\033[48;2;" << backgroundRed << ";" << backgroundGreen << ";" << backgroundBlue << "m" << text;
			return None::empty();
		}
	);

	/**
	 * @brief Allo input to be captured from the keyboard wihout echoing to the screen and without requiring the user to press enter
	 * 
	 */
	module->addFunction(
		"rawMode", {}, NONE, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			struct termios raw;
			tcgetattr(STDIN_FILENO, &holdTermios);
			raw = holdTermios;
			raw.c_lflag &= ~(ECHO);
			raw.c_lflag &= ~(ICANON);
			raw.c_cc[VMIN] = 0;
			raw.c_cc[VTIME] = 1;
			//hide the cursor
			std::cout << "\033[?25l";
			tcsetattr(STDIN_FILENO, TCSANOW, &raw);
			return None::empty();
		}
	);

	/**
	 * @brief Disable keyboard input capture
	 * 
	 */
	module->addFunction(
		"normalMode", {}, NONE, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			tcsetattr(STDIN_FILENO, TCSANOW , &holdTermios);
			//show the cursor
			std::cout << "\033[?25h";
			return None::empty();
		}
	);

	/**
	 * @brief get user input from the keyboard non-blocking (returns empty string if no input is available)
	 * 
	 */
	module->addFunction(
		"getKey", {}, STRING, [](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			char input;
			if (read(STDIN_FILENO, &input, 1) == 1) {
				return new String(std::string(1, input), TextRange(), Value::INTERPRETER);
			}
			return new String("", TextRange(), Value::INTERPRETER);
		}
	);

	module->addVariable("BLACK", new Int(BLACK, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("RED", new Int(RED, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("GREEN", new Int(GREEN, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("YELLOW", new Int(YELLOW, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("BLUE", new Int(BLUE, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("PURPLE", new Int(PURPLE, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("CYAN", new Int(CYAN, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("WHITE", new Int(WHITE, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("DEFAULT", new Int(DEFAULT, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("RESET", new Int(RESET, TextRange(), Value::CONTEXT_VARIABLE));

	module->addVariable("BLACK_BACKGROUND", new Int(40, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("RED_BACKGROUND", new Int(41, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("GREEN_BACKGROUND", new Int(42, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("YELLOW_BACKGROUND", new Int(43, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("BLUE_BACKGROUND", new Int(44, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("PURPLE_BACKGROUND", new Int(45, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("CYAN_BACKGROUND", new Int(46, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("WHITE_BACKGROUND", new Int(47, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("DEFAULT_BACKGROUND", new Int(49, TextRange(), Value::CONTEXT_VARIABLE));
	module->addVariable("RESET_BACKGROUND", new Int(0, TextRange(), Value::CONTEXT_VARIABLE));
	
	return result;
}