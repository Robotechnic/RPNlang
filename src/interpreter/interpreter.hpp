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
#include "codeblocks/blockqueue.hpp"
#include "codeblocks/line.hpp"
#include "codeblocks/codeblock.hpp"

#include "lexer/lexer.hpp"
#include "expressionresult/expressionresult.hpp"
#include "context/context.hpp"

#include "interpreter/memory.hpp"

#include "value/value.hpp"
#include "value/types.hpp"

#include "rpnfunctions/rpnfunction.hpp"
#include "rpnfunctions/builtinmap.hpp"

class Interpreter {
	public:
		Interpreter();
		Interpreter(Context *ctx);
		~Interpreter();

		bool interpretFile(std::string fileName, std::string &errorString);
		ExpressionResult interpretLine(std::string line, int lineNumber = 0);
		ExpressionResult interpret(BlockQueue &blocks);

		Value *getLastValue() const;
		Value *getReturnValue() const;


	private:
		bool openFile(std::ifstream &file, std::string fileName, std::string &error);

		ExpressionResult checkMemory();
		ExpressionResult interpretLine(Line &line);
		ExpressionResult interpretBlock(Line &line, CodeBlock &block);
		ExpressionResult interpretFString(FStringToken *token);
		ExpressionResult interpretOperator(Token *operatorToken);
		ExpressionResult interpretAssignment(Token *operatorToken);
		ExpressionResult interpretFunctionCall(Token *functionToken);
		
		Value *returnValue;
		Value *lastValue;
		
		TextRange mergeRanges(const std::vector<Value*> &values);
		void clearQueue(std::queue<Token*> &tokens);

		Memory memory;
		Context *context;
};