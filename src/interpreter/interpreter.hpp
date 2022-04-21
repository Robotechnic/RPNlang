#pragma once

#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <map>
#include <iostream>
#include "tokens/token.hpp"
#include "value/value.hpp"
#include "expressionresult/expressionresult.hpp"

class UserRPNFunction;

#include "rpnfunctions/rpnfunction.hpp"
#include "rpnfunctions/builtinsrpnfunction.hpp"
#include "rpnfunctions/userrpnfunction.hpp"

class Interpreter {
	public:
		Interpreter();
		Interpreter(std::map<std::string, Value> variables, std::map<std::string, RPNFunction*> functions);
		Interpreter(std::string fileName);
		~Interpreter();

		bool interpretFile(std::string &errorMessage);
		Value getLastValue() const;

		ExpressionResult interpret(std::string line);
		ExpressionResult interpret(std::vector<Token> tokens, int line);

	private:
		ExpressionResult applyOperator(const Token &mathOperator);
		ExpressionResult affectVariable(const Token &affectToken);
		ExpressionResult checkLiteral(const Token &literalToken);
		ExpressionResult createFunction(std::vector<Token> &tokens, Token affectToken, std::string body);
		std::string fileName;
		std::ifstream file;
		Value lastValue;

		void clearMemory();
		ExpressionResult checkMemory(int line);

		std::stack<Value> memory;
		std::map<std::string, Value> variables;
		std::map<std::string, RPNFunction*> functions; 
};