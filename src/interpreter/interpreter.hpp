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
#include "value/types/numbers/cppinterface.hpp"

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


	private:
		bool openFile(std::ifstream &file, std::string fileName, std::string &error);

		ExpressionResult checkMemory();
		ExpressionResult interpretLine(Line &line, bool clearMemory = true);
		ExpressionResult interpretBlock(Line &line, CodeBlock &block);
		ExpressionResult interpretFString(const FStringToken *token);
		ExpressionResult interpretOperator(const Token *operatorToken);
		ExpressionResult interpretKeyword(const Token *keywordToken);
		ExpressionResult interpretAssignment(const Token *operatorToken);
		ExpressionResult interpretFunctionCall(const Token *functionToken);
		ExpressionResult interpretIf(Line &line, CodeBlock &block);
		ExpressionResult interpretWhile(Line &line, CodeBlock &block);
		ExpressionResult interpretFor(Line &line, CodeBlock &block);
		ExpressionResult interpretTry(Line &line, CodeBlock &block);
		
		Value *lastValue;
		
		TextRange mergeRanges(const std::vector<Value*> &values);

		Memory memory;
		Context *context;
};