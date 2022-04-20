#include <iostream>
#include <sstream>
#include <string>
#include "expressionresult/expressionresult.hpp"
#include "interpreter/interpreter.hpp"

#include "rpnfunctions/builtinrpnfunction.cpp"

BuiltInRPNFunction<void(int, int)> function("Display line", std::vector<std::string>(2, std::string("ARG")), std::vector<ValueType>(2, ValueType::INT), ValueType::NONE, [](int line, int column) {
	std::cout << "Line " << line << " column " << column << std::endl;
});

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