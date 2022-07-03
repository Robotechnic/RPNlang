#include <iostream>
#include <filesystem>
#include <sstream>
#include <string>
#include <csignal>
#include "expressionresult/expressionresult.hpp"
#include "interpreter/interpreter.hpp"
#include "context/context.hpp"
#include "shell/colors.hpp"
#include "shell/shell.hpp"
#include "textutilities/textutilities.hpp"

#define TEST_FILE "/home/robotechnic/Documents/c++ projet/RPN language/tests/9 modules.rpn"

void shellInput() {
	Context *ctx = new Context("<stdin>");
	Interpreter i(ctx);
	Shell input;
	std::string instruction;
	input>>instruction;

	while (instruction != "exit") {
		ExpressionResult result = i.interpret(instruction);
		if (result.error()) {
			result.displayLineError(instruction);
		} else {
			switch (i.getLastValue().getType()) {
				case INT:
				case FLOAT:
					input<<MAGENTA;
					break;
				case STRING:
					input<<YELLOW;
					break;
				case BOOL:
					input<<GREEN;
					break;
				default:
					break;
			};
			input<<i.getLastValue().getStringValue()<<DEFAULT<<std::endl;
		}

		input>>instruction;
	}
	delete ctx;
}

void setWorkingDirectory(std::string path) {
	std::string extractedPath = extractFilePath(path);
	std::filesystem::current_path(extractedPath);
}

int main(int argc, char **argv) {
	bool result = true;
	#ifdef TEST_FILE
		argc = 0;
	#endif
	if (argc == 1) {
		shellInput();
	} else {
		#ifdef TEST_FILE
			std::string path = TEST_FILE;
		#else
			std::string path = argv[1];
		#endif
		setWorkingDirectory(path);
		Context *ctx = new Context(path, CONTEXT_TYPE_FILE);
		result = Interpreter(ctx).interpretFile(path);
		delete ctx;
	}

	return result ? 0 : 1;
}