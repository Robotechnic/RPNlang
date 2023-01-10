#pragma once

#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "textutilities/textutilities.hpp"

#include "tokens/token.hpp"
#include "tokens/tokens/typetoken.hpp"
#include "tokens/tokens/keywordtoken.hpp"
#include "codeblocks/blockqueue.hpp"
#include "codeblocks/line.hpp"
#include "codeblocks/codeblock.hpp"
#include "codeblocks/functionblock.hpp"
#include "codeblocks/structblock.hpp"

#include "value/value.hpp"
#include "value/types.hpp"
#include "value/types/numbers/cppinterface.hpp"

#include "lexer/lexer.hpp"
#include "expressionresult/expressionresult.hpp"
#include "context/context.hpp"

#include "interpreter/memory.hpp"

#include "rpnfunctions/rpnfunction.hpp"
#include "rpnfunctions/builtinmap.hpp"

class Interpreter {
	public:
		Interpreter();
		Interpreter(ContextPtr ctx);
		~Interpreter();

		bool interpretFile(std::string_view fileName, std::string &errorString, bool isModule = false);
		ExpressionResult interpretLine(std::string_view line, int lineNumber = 0);
		ExpressionResult interpret(BlockQueue &blocks);

		Value *getLastValue() const;

	private:
		ExpressionResult checkMemory();
		ExpressionResult interpretLine(Line &line, bool clearMemory = true);
		ExpressionResult interpretBlock(Line &line, CodeBlock &block);
		ExpressionResult interpretFString(const FStringToken *token);
		ExpressionResult interpretOperator(const Token *operatorToken);
		ExpressionResult interpretKeyword(const Token *keywordToken);
		ExpressionResult interpretValueType(const Token *typeToken);
		ExpressionResult interpretAssignment(const Token *operatorToken);
		ExpressionResult getFunction(const Value *functionName, const RPNFunction *&function);
		ExpressionResult interpretFunctionCall(Token *functionToken);
		ExpressionResult interpretIf(Line &line, CodeBlock &block);
		ExpressionResult interpretWhile(Line &line, CodeBlock &block);
		ExpressionResult interpretFor(Line &line, CodeBlock &block);
		ExpressionResult interpretTry(Line &line, CodeBlock &block);
		ExpressionResult interpretList(const Token *keywordToken);
		TextRange mergeRanges(const std::vector<Value*> &values);

		Value *lastValue;
		Memory memory;
		ContextPtr context;
};