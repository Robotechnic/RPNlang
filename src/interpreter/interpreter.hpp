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
#include "textutilities/escapecharacters.hpp"
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
		Interpreter(Context *ctx);
		~Interpreter();

		bool interpretFile(std::string fileName);
		Value getLastValue() const;
		Value getReturnValue() const;

		ExpressionResult interpret(std::string line, int lineNumber = 0);
		ExpressionResult interpret(std::queue<Token> tokens);

	private:
		bool openFile(std::ifstream &file, std::string fileName);
		TextRange mergeRanges(const std::vector<Value> &values);

		ExpressionResult interpretToken(const Token &tok, std::queue<Token> &tokens, std::queue<Token> &previous);
		ExpressionResult applyOperator(const Token &mathOperator);
		ExpressionResult affectVariable(const Token &affectToken);

		ExpressionResult isFunction(const Token &functionName, bool &builtin, int &argCount);
		ExpressionResult checkArgs(const Token &literalToken, int argCount, RPNFunctionArgs &args);
		ExpressionResult callFunction(const Token &functionName);
		
		ExpressionResult extractFStringSubstrings(const Token &fStringToken, std::vector<std::string> &substrings);
		ExpressionResult parseFString(const Token &fStringToken);

		ExpressionResult extractExpressionBody(const Token &keywordToken, std::queue<Token> &tokens, std::queue<Token> &expressionBody, Token &expressionEnd);
		ExpressionResult extractExpressionBody(const Token &keywordToken, std::queue<Token> &tokens, std::queue<Token> &expressionBody, std::string expressionEnd);
		ExpressionResult getUntilToken(const Token &keywordToken, std::queue<Token> &tokens, std::queue<Token> &expressionBody, TokenType tokenType, std::string tokenValue);

		ExpressionResult parseKeyword(const Token &keywordToken, std::queue<Token> &tokens, const std::queue<Token> &previous);
		ExpressionResult parseIf(const Token &keywordToken, std::queue<Token> &tokens);
		ExpressionResult parseElse(const Token &keywordToken, std::queue<Token> &tokens, bool skipElse);

		ExpressionResult parseForParameters(const Token &keywordToken, std::string &incrementName, Value range[3]);
		ExpressionResult parseFor(const Token &keywordToken, std::queue<Token> &tokens);

		ExpressionResult parseWhile(const Token &keywordToken, std::queue<Token> &tokens, const std::queue<Token> &previous);

		ExpressionResult extractFunctionArgs(const Token &keywordToken, std::queue<Token> &tokens, std::string &name, std::vector<std::string> &argsNames, std::vector<ValueType> &types, ValueType &returnType);
		ExpressionResult createFunction(const Token &keywordToken, std::queue<Token> &tokens);

		ExpressionResult parseFunctionCall(const Token &keywordToken, std::queue<Token> &tokens);

		ExpressionResult parseReturn(const Token &keywordToken, std::queue<Token> &tokens);
		ExpressionResult parseContinueBreak(const Token &keywordToken);

		ExpressionResult parseTry(const Token &keywordToken, std::queue<Token> &tokens);
		ExpressionResult parseCatch(const Token &keywordToken, std::queue<Token> &tokens, ExpressionResult &tryResult);
		ExpressionResult parseFinally(const Token &keywordToken, std::queue<Token> &tokens);


		Value returnValue;
		Value lastValue;
		
		void clearQueue(std::queue<Token> &tokens);
		void clearMemory();
		ExpressionResult checkMemory();

		std::stack<Value> memory;
		Context *context;
		bool quit;
		int loopLevel;
};