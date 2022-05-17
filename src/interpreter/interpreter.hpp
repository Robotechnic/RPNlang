#pragma once

#include <cstring>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <fstream>
#include <map>
#include <iostream>
#include <algorithm>
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
		Interpreter(Context ctx);
		~Interpreter();

		bool interpretFile(std::string fileName);
		Value getLastValue() const;

		ExpressionResult interpret(std::string line, int lineNumber = 0);
		ExpressionResult interpret(std::queue<Token> tokens);

	private:
		TextRange mergeRanges(const std::vector<Value> &values);

		ExpressionResult interpretToken(const Token &tok, std::queue<Token> &tokens);
		ExpressionResult applyOperator(const Token &mathOperator);
		ExpressionResult affectVariable(const Token &affectToken);

		ExpressionResult isFunction(const Token &functionName, bool &builtin, int &argCount);
		ExpressionResult checkArgs(const Token &literalToken, int argCount, RPNFunctionArgs &args);
		ExpressionResult callFunction(const Token &functionName);
		
		ExpressionResult extractFStringSubstrings(const Token &fStringToken, std::vector<std::string> &substrings);
		ExpressionResult parseFString(const Token &fStringToken);

		ExpressionResult parseKeyword(const Token &keywordToken, std::queue<Token> &tokens);
		ExpressionResult parseIf(const Token &keywordToken, std::queue<Token> &tokens);
		ExpressionResult parseElse(const Token &keywordToken, std::queue<Token> &tokens, bool skipElse);

		ExpressionResult parseForParameters(const Token &keywordToken, std::string &incrementName, Value range[3]);
		ExpressionResult parseFor(const Token &keywordToken, std::queue<Token> &tokens);

		// not implemented yet
		ExpressionResult createFunction(const Token &keywordToken, std::queue<Token> &tokens);
		

		Value lastValue;

		void skipSeparators(std::queue<Token> &tokens);
		void clearMemory();
		ExpressionResult checkMemory();


		ExpressionResult extractExpressionArguments(std::queue<Token> &tokens, std::queue<Token> &expressionArguments);
		ExpressionResult extractExpressionBody(std::queue<Token> &tokens, std::queue<Token> &expressionBody, Token &expressionEnd);

		std::stack<Value> memory;
		Context context;
};