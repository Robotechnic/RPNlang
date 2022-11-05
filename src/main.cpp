#include <iostream>
#include <filesystem>
#include <sstream>
#include <string>
#include <csignal>

#include "textutilities/textutilities.hpp"
#include "expressionresult/expressionresult.hpp"
#include "interpreter/interpreter.hpp"
#include "context/context.hpp"
#include "shell/colors.hpp"
#include "shell/shell.hpp"

// #define TEST_FILE "/home/robotechnic/Documents/c++ projet/RPN language/tests/3 whileLoops.rpn"

/**
 * @brief allow shell to be destroyed when ctrl+c is pressed, this allow to save the history
 * 
 * @param signum signal number
 */
void signalHandler(int signum) {
	if (signum == SIGINT) return;
	std::cout<<std::endl;
	exit(signum);
}

void shellInput() {
	Context *ctx = new Context("main", "<stdin>");
	Interpreter i(ctx);
	std::string instruction;
	rpnShell>>instruction;

	while (instruction != "exit") {
		ExpressionResult result = i.interpretLine(instruction);
		if (result.error()) {
			result.displayLineError(instruction);
		} else {
			switch (i.getLastValue()->getType()) {
				case INT:
				case FLOAT:
					rpnShell<<MAGENTA;
					break;
				case STRING:
					rpnShell<<YELLOW;
					break;
				case BOOL:
					rpnShell<<GREEN;
					break;
				case NONE:
					rpnShell<<RED;
					break;
				default:
					break;
			};
			rpnShell<<i.getLastValue()->getStringValue()<<DEFAULT<<std::endl;
		}

		rpnShell>>instruction;
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
		signal(SIGTERM, signalHandler);
		signal(SIGINT, signalHandler);
		rpnShell.loadHistory();
		shellInput();
	} else {
		#ifdef TEST_FILE
			std::string path = TEST_FILE;
		#else
			std::string path = argv[1];
		#endif
		setWorkingDirectory(path);
		Context *ctx = new Context(extractFileName(path), path, CONTEXT_TYPE_FILE);
		std::string error;
		result = Interpreter(ctx).interpretFile(path, error);
		if (!result) {
			std::cout << error << std::endl;
		}
		delete ctx;
	}

	return result ? 0 : 1;
}