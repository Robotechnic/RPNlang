#pragma once

#include <string>
#include <vector>
#include <stack>
#include <queue>
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
		Interpreter(std::map<std::string, Value> variables);
		Interpreter(std::string fileName);
		~Interpreter();

		bool interpretFile(std::string &errorMessage);
		Value getLastValue() const;

		ExpressionResult interpret(std::string line);
		ExpressionResult interpret(std::queue<Token> tokens, int line);

	private:
		ExpressionResult applyOperator(const Token &mathOperator);
		ExpressionResult affectVariable(const Token &affectToken);
		ExpressionResult checkLiteral(const Token &literalToken);
		ExpressionResult isFunction(const Token &functionName, std::string name, bool &builtin, int &argCount);
		ExpressionResult checkArgs(const Token &literalToken, int argCount, RPNFunctionArgs &args);
		ExpressionResult callFunction(const Token &functionName);
		ExpressionResult parseKeyword(const Token &keywordToken, std::queue<Token> &tokens);
		ExpressionResult createFunction(const Token &keywordToken, std::queue<Token> &tokens);
		std::string fileName;
		std::ifstream file;
		Value lastValue;

		void clearMemory();
		ExpressionResult checkMemory(int line);

		std::stack<Value> memory;
		std::map<std::string, Value> variables;
};