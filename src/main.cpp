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
			result.display(instruction);
		} else {
			std::cout<<i.getLastValue().getStringValue()<<std::endl;
		}

		readLine(instruction);
	}
}

int main(int argc, char **argv) {
	if (argc == 1) {
		shell();
	} else {
		return Interpreter(Context(argv[1], CONTEXT_TYPE_FILE)).interpretFile(argv[1]);
	}

	return 0;
}