#include "terminal.hpp"

ExpressionResult loader(CppModule *module) {
	ExpressionResult result;
	
	module->addFunction(
		"width", {}, {}, INT, [](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			struct winsize w;
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			return std::make_tuple(ExpressionResult(), new Int(w.ws_col, range, false));
		}
	);

	module->addFunction(
		"height", {}, {}, INT, [](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			struct winsize w;
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			return std::make_tuple(ExpressionResult(), new Int(w.ws_row, range, false));
		}
	);

	module->addFunction(
		"clear", {}, {}, NONE, [](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			// reset color and clear screen
			std::cout << "\033[0m";
			std::cout << "\033[2J\033[0;0H";
			return std::make_tuple(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"setPixelColor", {"x", "y", "color"}, {INT, INT, INT}, NONE, [](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			int x = static_cast<Int*>(args[0])->getValue();
			int y = static_cast<Int*>(args[1])->getValue();
			int pixel = static_cast<Int*>(args[2])->getValue();
			if (pixel < 40 || pixel > 49) {
				return std::make_tuple(
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
			return std::make_tuple(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"setRGBPixelColor", {"x","y","red", "blue", "green"}, {INT, INT, INT, INT, INT}, NONE, [](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			int red = static_cast<Int*>(args[2])->getValue();
			int blue = static_cast<Int*>(args[3])->getValue();
			int green = static_cast<Int*>(args[4])->getValue();
			int x = static_cast<Int*>(args[0])->getValue();
			int y = static_cast<Int*>(args[1])->getValue();
			if (red < 0 || red > 255 || blue < 0 || blue > 255 || green < 0 || green > 255) {
				return std::make_tuple(
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
			return std::make_tuple(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"setAscii", {"x", "y", "letter", "color", "background"}, {INT, INT, STRING, INT, INT}, NONE, [](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			std::string letter = args[0]->getStringValue();
			int x = static_cast<Int*>(args[1])->getValue();
			int y = static_cast<Int*>(args[2])->getValue();
			int color = static_cast<Int*>(args[3])->getValue();
			int background = static_cast<Int*>(args[4])->getValue();
			if (color < 30 || color > 37) {
				return std::make_tuple(
					ExpressionResult(
						"Color must be between 30 and 37",
						args[3]->getRange(),
						context
					),
					None::empty()
				);
			}
			if (background < 40 || background > 47) {
				return std::make_tuple(
					ExpressionResult(
						"Background must be between 40 and 47",
						args[4]->getRange(),
						context
					),
					None::empty()
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[" << color << ";" << background << "m" << letter;
			return std::make_tuple(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"setRGBAscii", {"x", "y", "letter", "red", "blue", "green", "backgroundRed", "backgroundBlue", "backgroundGreen"}, {INT, INT, STRING, INT, INT, INT, INT, INT, INT}, NONE, [](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			std::string letter = args[0]->getStringValue();
			int x = static_cast<Int*>(args[1])->getValue();
			int y = static_cast<Int*>(args[2])->getValue();
			int red = static_cast<Int*>(args[3])->getValue();
			int blue = static_cast<Int*>(args[4])->getValue();
			int green = static_cast<Int*>(args[5])->getValue();
			int backgroundRed = static_cast<Int*>(args[6])->getValue();
			int backgroundBlue = static_cast<Int*>(args[7])->getValue();
			int backgroundGreen = static_cast<Int*>(args[8])->getValue();
			if (red < 0 || red > 255 || blue < 0 || blue > 255 || green < 0 || green > 255) {
				return std::make_tuple(
					ExpressionResult(
						"Pixel color must be between 0 and 255",
						red < 0 || red > 255 ? args[0]->getRange() : (blue < 0 || blue > 255 ? args[1]->getRange() : args[2]->getRange()),
						context
					),
					None::empty()
				);
			}
			if (backgroundRed < 0 || backgroundRed > 255 || backgroundBlue < 0 || backgroundBlue > 255 || backgroundGreen < 0 || backgroundGreen > 255) {
				return std::make_tuple(
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
			std::cout << "\033[48;2;" << backgroundRed << ";" << backgroundGreen << ";" << backgroundBlue << "m" << letter;
			return std::make_tuple(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"drawText", {"x", "y", "text", "color", "background"}, {INT, INT, STRING, INT, INT}, NONE, [](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			std::string text = args[0]->getStringValue();
			int x = static_cast<Int*>(args[1])->getValue();
			int y = static_cast<Int*>(args[2])->getValue();
			int color = static_cast<Int*>(args[3])->getValue();
			int background = static_cast<Int*>(args[4])->getValue();
			if (color < 30 || color > 37) {
				return std::make_tuple(
					ExpressionResult(
						"Color must be between 30 and 37",
						args[3]->getRange(),
						context
					),
					None::empty()
				);
			}
			if (background < 40 || background > 47) {
				return std::make_tuple(
					ExpressionResult(
						"Background must be between 40 and 47",
						args[4]->getRange(),
						context
					),
					None::empty()
				);
			}
			std::cout << "\033[" << y << ";" << x << "H";
			std::cout << "\033[" << color << ";" << background << "m" << text;
			return std::make_tuple(ExpressionResult(), None::empty());
		}
	);

	module->addFunction(
		"drawTextRGB", {"x", "y", "text", "red", "blue", "green", "backgroundRed", "backgroundBlue", "backgroundGreen"}, {INT, INT, STRING, INT, INT, INT, INT, INT, INT}, NONE, [](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			std::string text = args[0]->getStringValue();
			int x = static_cast<Int*>(args[1])->getValue();
			int y = static_cast<Int*>(args[2])->getValue();
			int red = static_cast<Int*>(args[3])->getValue();
			int blue = static_cast<Int*>(args[4])->getValue();
			int green = static_cast<Int*>(args[5])->getValue();
			int backgroundRed = static_cast<Int*>(args[6])->getValue();
			int backgroundBlue = static_cast<Int*>(args[7])->getValue();
			int backgroundGreen = static_cast<Int*>(args[8])->getValue();
			if (red < 0 || red > 255 || blue < 0 || blue > 255 || green < 0 || green > 255) {
				return std::make_tuple(
					ExpressionResult(
						"Pixel color must be between 0 and 255",
						red < 0 || red > 255 ? args[0]->getRange() : (blue < 0 || blue > 255 ? args[1]->getRange() : args[2]->getRange()),
						context
					),
					None::empty()
				);
			}
			if (backgroundRed < 0 || backgroundRed > 255 || backgroundBlue < 0 || backgroundBlue > 255 || backgroundGreen < 0 || backgroundGreen > 255) {
				return std::make_tuple(
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
			return std::make_tuple(ExpressionResult(), None::empty());
		}
	);

	module->addVariable("BLACK", new Int(30, TextRange(), false));
	module->addVariable("RED", new Int(31, TextRange(), false));
	module->addVariable("GREEN", new Int(32, TextRange(), false));
	module->addVariable("YELLOW", new Int(33, TextRange(), false));
	module->addVariable("BLUE", new Int(34, TextRange(), false));
	module->addVariable("MAGENTA", new Int(35, TextRange(), false));
	module->addVariable("CYAN", new Int(36, TextRange(), false));
	module->addVariable("WHITE", new Int(37, TextRange(), false));
	module->addVariable("DEFAULT", new Int(39, TextRange(), false));
	module->addVariable("RESET", new Int(0, TextRange(), false));

	module->addVariable("BLACK_BACKGROUND", new Int(40, TextRange(), false));
	module->addVariable("RED_BACKGROUND", new Int(41, TextRange(), false));
	module->addVariable("GREEN_BACKGROUND", new Int(42, TextRange(), false));
	module->addVariable("YELLOW_BACKGROUND", new Int(43, TextRange(), false));
	module->addVariable("BLUE_BACKGROUND", new Int(44, TextRange(), false));
	module->addVariable("MAGENTA_BACKGROUND", new Int(45, TextRange(), false));
	module->addVariable("CYAN_BACKGROUND", new Int(46, TextRange(), false));
	module->addVariable("WHITE_BACKGROUND", new Int(47, TextRange(), false));
	module->addVariable("DEFAULT_BACKGROUND", new Int(49, TextRange(), false));
	module->addVariable("RESET_BACKGROUND", new Int(0, TextRange(), false));
	
	return result;
}