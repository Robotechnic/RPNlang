#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <stack>
#include <iostream>
#include "tokens/token.hpp"
#include "value/value.hpp"
#include "expressionresult/expressionresult.hpp"

class RPNFunction;

#include "rpnfunctions/rpnfunction.hpp"

class Interpreter {
	public:
		Interpreter();
		Interpreter(std::map<std::string, Value> variables, std::map<std::string, RPNFunction> functions);
		Interpreter(std::string fileName);
		~Interpreter();

		bool interpretFile(std::string &errorMessage);

		ExpressionResult interpret(std::string line);
		ExpressionResult interpret(std::vector<Token> &tokens);

	private:
		std::string fileName;
		std::ifstream file;
		std::map<std::string, Value> variables;
		std::map<std::string, RPNFunction> functions; 
};