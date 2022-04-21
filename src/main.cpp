#include <iostream>
#include <sstream>
#include <string>
#include "expressionresult/expressionresult.hpp"
#include "interpreter/interpreter.hpp"

void readLine(std::string &line) {
	line.clear();
	std::cout<<">>> ";
	std::getline(std::cin, line);
}

int main() {
	Interpreter i;
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

	return 0;
}