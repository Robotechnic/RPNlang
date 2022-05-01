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

class Context;
#include "context/context.hpp"

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
		TextRange mergeRanges(const std::vector<Value> &values);

		ExpressionResult interpretToken(const Token &tok, std::queue<Token> &tokens, int line);
		ExpressionResult applyOperator(const Token &mathOperator);
		ExpressionResult affectVariable(const Token &affectToken);
		ExpressionResult isFunction(const Token &functionName, bool &builtin, int &argCount);
		ExpressionResult checkArgs(const Token &literalToken, int argCount, RPNFunctionArgs &args);
		ExpressionResult callFunction(const Token &functionName);
		
		ExpressionResult extractFStringSubstrings(const Token &fStringToken, std::vector<std::string> &substrings);
		ExpressionResult parseFString(const Token &fStringToken);


		// not implemented yet
		ExpressionResult parseKeyword(const Token &keywordToken, std::queue<Token> &tokens);
		ExpressionResult createFunction(const Token &keywordToken, std::queue<Token> &tokens);
		ExpressionResult parseIf(const Token &keywordToken, std::queue<Token> &tokens);
		ExpressionResult parseElse(std::queue<Token> &tokens);



		std::string fileName;
		std::ifstream file;
		Value lastValue;

		void skipSeparators(std::queue<Token> &tokens);
		void clearMemory();
		ExpressionResult checkMemory(int line);


		ExpressionResult extractExpressionArguments(std::queue<Token> &tokens, std::queue<Token> &expressionArguments);
		ExpressionResult extractExpressionBody(std::queue<Token> &tokens, std::queue<Token> &expressionBody);

		std::stack<Value> memory;
		std::map<std::string, Value> variables;
};