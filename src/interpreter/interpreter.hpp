#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <stack>
#include <iostream>
#include "tokens/token.hpp"
#include "interpreter/value.hpp"

class Interpreter {
	public:
		Interpreter();
		Interpreter(std::string fileName);
		~Interpreter();

		bool interpretFile(std::string &errorMessage);

		bool interpret(std::string line, std::string &errorMessage);
		bool interpret(std::vector<Token> &tokens, std::string &errorMessage);

	private:
		std::string fileName;
		std::ifstream file;
		std::map<std::string, Value> variables;

		bool checkVariable();

		// std::map<std::string, Function> functions; 
};