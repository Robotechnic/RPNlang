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
	rpnShell.loadHistory();
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
			rpnShell<<"\n"<<i.getLastValue()->getStringValue()<<DEFAULT<<std::endl;
		}

		rpnShell>>instruction;
	}
	delete ctx;
}

void setWorkingDirectory(std::string path) {
	try {
		std::string extractedPath = extractFilePath(path);
		if (extractedPath[0] == '/') {
			std::filesystem::current_path(extractedPath);
		} else {
			std::filesystem::current_path(std::filesystem::current_path().string() + "/" + extractedPath);
		}
	} catch (std::filesystem::filesystem_error &e) {
		std::cout<<"Error: "<<e.what()<<std::endl;
		exit(1);
	}
}

int main(int argc, char **argv) {
	bool result = true;
	#ifdef TEST_FILE
		argc = 0;
	#endif
	if (argc == 1) {
		signal(SIGTERM, signalHandler);
		signal(SIGINT, signalHandler);
		shellInput();
	} else {
		#ifdef TEST_FILE
			std::string path = TEST_FILE;
		#else
			std::string path = argv[1];
		#endif
		setWorkingDirectory(path);
		std::string name = extractFileName(path);
		path = path.substr(path.find_last_of('/') + 1);
		Context *ctx = new Context(name, path, CONTEXT_TYPE_FILE);
		std::string error;
		result = Interpreter(ctx).interpretFile(path, error);
		if (!result) {
			std::cout << error << std::endl;
		}
		delete ctx;
	}

	return result ? 0 : 1;
}