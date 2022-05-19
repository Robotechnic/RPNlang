#include <iostream>
#include <sstream>
#include <string>
#include "expressionresult/expressionresult.hpp"
#include "interpreter/interpreter.hpp"
#include "context/context.hpp"

void readLine(std::string &line) {
	line.clear();
	std::cout<<">>> ";
	std::getline(std::cin, line);
}

void shell() {
	Interpreter i(Context("<stdin>"));
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
	if (argc == 1 && false) {
		shell();
	} else {
		// std::string path = argv[1];
		std::string path = "/home/robotechnic/Documents/c++ projet/RPN language/examples/while.rpn";

		return Interpreter(Context(path, CONTEXT_TYPE_FILE)).interpretFile(path);
	}

	return 0;
}