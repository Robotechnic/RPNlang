#include <iostream>
#include <sstream>
#include <string>
#include "expressionresult/expressionresult.hpp"
#include "interpreter/interpreter.hpp"
#include "context/context.hpp"

// #define TEST_FILE "/home/robotechnic/Documents/c++ projet/RPN language/examples/functions.rpn"

void readLine(std::string &line) {
	line.clear();
	std::cout<<">>> ";
	std::getline(std::cin, line);
}

void shell() {
	Interpreter i(new Context("<stdin>"));
	std::string errorMessage;
	std::vector<Token> tokens;
	std::string instruction;
	readLine(instruction);
	while (instruction != "exit") {
		ExpressionResult result = i.interpret(instruction);
		if (result.error()) {
			result.displayLineError(instruction);
		} else {
			std::cout<<i.getLastValue().getStringValue()<<std::endl;
		}

		readLine(instruction);
	}
}

int main(int argc, char **argv) {
	bool result = true;
	#ifdef TEST_FILE
		argc = 0;
	#endif
	if (argc == 1) {
		shell();
	} else {
		#ifdef TEST_FILE
			std::string path = TEST_FILE;
		#else
			std::string path = argv[1];
		#endif
		Context *ctx = new Context(path, CONTEXT_TYPE_FILE);
		result = Interpreter(ctx).interpretFile(path);
		delete ctx;
	}

	return result ? 0 : 1;
}