#include <iostream>
#include <sstream>
#include <string>
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
		if (!i.interpret(instruction, errorMessage)) {
			std::cout<<"Error : "<<errorMessage<<std::endl;
		}

		readLine(instruction);
	}

	return 0;
}