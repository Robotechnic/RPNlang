#include "terminal.hpp"

struct termios holdTermios;

ExpressionResult loader(CppModule *module) {
	ExpressionResult result;
	
	module->addFunction(
		"width", {}, {}, INT, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			struct winsize w;
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			return std::make_pair(ExpressionResult(), new Int(w.ws_col, range, Value::INTERPRETER));
		}
	);

	module->addFunction(
		"height", {}, {}, INT, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			struct winsize w;
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			return std::make_pair(ExpressionResult(), new Int(w.ws_row, range, Value::INTERPRETER));
		}
	);

	module->addFunction(
		"clear", {}, {}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			// reset color and clear screen
			std::cout << "\033[0m";
			std::cout << "\033[2J";
			struct winsize w;
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			for (int i = 0; i < w.ws_row; i++) {
				std::cout << "\033[" << i << ";0H ";
			}
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"home", {}, {}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			// set cursor to top left of screen and reset colors
			std::cout << "\033[0;0H";
			std::cout << "\033[0m";
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"end", {}, {}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			// set cursor to bottom of screen
			std::cout << "\033[9999;9999H";
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"flush", {}, {}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			// flush output
			std::cout << std::flush;
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"setPixelColor", {"x", "y", "color"}, {INT, INT, INT}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			int x = static_cast<Int*>(args[0])->getValue();
			int y = static_cast<Int*>(args[1])->getValue();
			int pixel = static_cast<Int*>(args[2])->getValue();
			if (pixel < 40 || pixel > 49) {
				return std::make_pair(
					ExpressionResult(
						"Pixel color must be between 40 and 49",
						args[2]->getRange(),
						context
					),
					None::empty()
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[" << pixel << "m ";
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"setRGBPixelColor", {"x","y","red", "blue", "green"}, {INT, INT, INT, INT, INT}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			int red = static_cast<Int*>(args[2])->getValue();
			int blue = static_cast<Int*>(args[3])->getValue();
			int green = static_cast<Int*>(args[4])->getValue();
			int x = static_cast<Int*>(args[0])->getValue();
			int y = static_cast<Int*>(args[1])->getValue();
			if (red < 0 || red > 255 || blue < 0 || blue > 255 || green < 0 || green > 255) {
				return std::make_pair(
					ExpressionResult(
						"Pixel color must be between 0 and 255",
						red < 0 || red > 255 ? args[0]->getRange() : (blue < 0 || blue > 255 ? args[1]->getRange() : args[2]->getRange()),
						context
					),
					None::empty()
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[48;2;" << red << ";" << green << ";" << blue << "m ";
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"setAscii", {"x", "y", "letter", "color", "background"}, {INT, INT, STRING, INT, INT}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			const std::string letter = args[0]->getStringValue();
			int x = static_cast<Int*>(args[1])->getValue();
			int y = static_cast<Int*>(args[2])->getValue();
			int color = static_cast<Int*>(args[3])->getValue();
			int background = static_cast<Int*>(args[4])->getValue();
			if (color < 30 || color > 39) {
				return std::make_pair(
					ExpressionResult(
						"Color must be between 30 and 39",
						args[3]->getRange(),
						context
					),
					None::empty()
				);
			}
			if (background < 40 || background > 49) {
				return std::make_pair(
					ExpressionResult(
						"Background must be between 40 and 49",
						args[4]->getRange(),
						context
					),
					None::empty()
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[" << color << ";" << background << "m" << letter;
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"setRGBAscii", {"x", "y", "letter", "red", "blue", "green", "backgroundRed", "backgroundBlue", "backgroundGreen"}, {INT, INT, STRING, INT, INT, INT, INT, INT, INT}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			const std::string letter = args[0]->getStringValue();
			int x = static_cast<Int*>(args[1])->getValue();
			int y = static_cast<Int*>(args[2])->getValue();
			int red = static_cast<Int*>(args[3])->getValue();
			int blue = static_cast<Int*>(args[4])->getValue();
			int green = static_cast<Int*>(args[5])->getValue();
			int backgroundRed = static_cast<Int*>(args[6])->getValue();
			int backgroundBlue = static_cast<Int*>(args[7])->getValue();
			int backgroundGreen = static_cast<Int*>(args[8])->getValue();
			if (red < 0 || red > 255 || blue < 0 || blue > 255 || green < 0 || green > 255) {
				return std::make_pair(
					ExpressionResult(
						"Pixel color must be between 0 and 255",
						red < 0 || red > 255 ? args[0]->getRange() : (blue < 0 || blue > 255 ? args[1]->getRange() : args[2]->getRange()),
						context
					),
					None::empty()
				);
			}
			if (backgroundRed < 0 || backgroundRed > 255 || backgroundBlue < 0 || backgroundBlue > 255 || backgroundGreen < 0 || backgroundGreen > 255) {
				return std::make_pair(
					ExpressionResult(
						"Pixel color must be between 0 and 255",
						backgroundRed < 0 || backgroundRed > 255 ? args[3]->getRange() : (backgroundBlue < 0 || backgroundBlue > 255 ? args[4]->getRange() : args[5]->getRange()),
						context
					),
					None::empty()
				);
			}
			if (letter.size() != 1) {
				return std::make_pair(
					ExpressionResult(
						"Letter must be a single character",
						args[0]->getRange(),
						context
					),
					None::empty()
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[38;2;" << red << ";" << green << ";" << blue << "m";
			std::cout << "\033[48;2;" << backgroundRed << ";" << backgroundGreen << ";" << backgroundBlue << "m" << letter;
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"drawRect", {"x", "y", "width", "height", "color"}, {INT, INT, INT, INT, INT}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			int x = static_cast<Int*>(args[0])->getValue();
			int y = static_cast<Int*>(args[1])->getValue();
			int width = static_cast<Int*>(args[2])->getValue();
			int height = static_cast<Int*>(args[3])->getValue();
			int color = static_cast<Int*>(args[4])->getValue();
			if (color < 30 || color > 49) {
				return std::make_pair(
					ExpressionResult(
						"Color must be between 40 and 49",
						args[4]->getRange(),
						context
					),
					None::empty()
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[" << color << "m";
			for (int i = 0; i < height; i++) {
				std::cout << "\033[" << i << "C";
				for (int j = 0; j < width; j++) {
					std::cout << " ";
				}
			}
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"drawText", {"x", "y", "text", "color", "background"}, {INT, INT, STRING, INT, INT}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			const std::string text = args[2]->getStringValue();
			int x = static_cast<Int*>(args[0])->getValue();
			int y = static_cast<Int*>(args[1])->getValue();
			int color = static_cast<Int*>(args[3])->getValue();
			int background = static_cast<Int*>(args[4])->getValue();
			if (color < 30 || color > 39) {
				return std::make_pair(
					ExpressionResult(
						"Color must be between 30 and 39",
						args[3]->getRange(),
						context
					),
					None::empty()
				);
			}
			if (background < 40 || background > 49) {
				return std::make_pair(
					ExpressionResult(
						"Background must be between 40 and 49",
						args[4]->getRange(),
						context
					),
					None::empty()
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[" << color << ";" << background << "m" << text;
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"drawTextRGB", {"x", "y", "text", "red", "blue", "green", "backgroundRed", "backgroundBlue", "backgroundGreen"}, {INT, INT, STRING, INT, INT, INT, INT, INT, INT}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
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
				return std::make_pair(
					ExpressionResult(
						"Pixel color must be between 0 and 255",
						red < 0 || red > 255 ? args[0]->getRange() : (blue < 0 || blue > 255 ? args[1]->getRange() : args[2]->getRange()),
						context
					),
					None::empty()
				);
			}
			if (backgroundRed < 0 || backgroundRed > 255 || backgroundBlue < 0 || backgroundBlue > 255 || backgroundGreen < 0 || backgroundGreen > 255) {
				return std::make_pair(
					ExpressionResult(
						"Pixel color must be between 0 and 255",
						backgroundRed < 0 || backgroundRed > 255 ? args[3]->getRange() : (backgroundBlue < 0 || backgroundBlue > 255 ? args[4]->getRange() : args[5]->getRange()),
						context
					),
					None::empty()
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[38;2;" << red << ";" << green << ";" << blue << "m";
			std::cout << "\033[48;2;" << backgroundRed << ";" << backgroundGreen << ";" << backgroundBlue << "m" << text;
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	/**
	 * @brief Allo input to be captured from the keyboard wihout echoing to the screen and without requiring the user to press enter
	 * 
	 */
	module->addFunction(
		"rawMode", {}, {}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
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
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	/**
	 * @brief Disable keyboard input capture
	 * 
	 */
	module->addFunction(
		"normalMode", {}, {}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			tcsetattr(STDIN_FILENO, TCSANOW , &holdTermios);
			//show the cursor
			std::cout << "\033[?25h";
			return std::make_pair(ExpressionResult(), None::empty());
		}
	);

	/**
	 * @brief get user input from the keyboard non-blocking (returns empty string if no input is available)
	 * 
	 */
	module->addFunction(
		"getKey", {}, {}, STRING, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			char input;
			if (read(STDIN_FILENO, &input, 1) == 1) {
				return std::make_pair(ExpressionResult(), new String(std::string(1, input), TextRange(), Value::INTERPRETER));
			}
			return std::make_pair(ExpressionResult(), new String("", TextRange(), Value::INTERPRETER));
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