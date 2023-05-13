#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "textutilities/textutilities.hpp"

#include "codeblocks/blockqueue.hpp"
#include "codeblocks/codeblock.hpp"
#include "codeblocks/functionblock.hpp"
#include "codeblocks/line.hpp"
#include "tokens/token.hpp"
#include "tokens/tokens/keywordtoken.hpp"
#include "tokens/tokens/typetoken.hpp"

#include "value/types.hpp"
#include "value/types/listelement.hpp"
#include "value/types/numbers/cppinterface.hpp"
#include "value/value.hpp"

#include "context/context.hpp"
#include "expressionresult/expressionresult.hpp"
#include "lexer/lexer.hpp"

#include "interpreter/memory.hpp"

#include "rpnfunctions/builtinmap.hpp"
#include "rpnfunctions/rpnfunction.hpp"

class Interpreter {
  public:
	Interpreter();
	Interpreter(ContextPtr ctx);
	~Interpreter();

	void clearMemory();

	bool interpretFile(std::string_view fileName, std::string &errorString, bool isModule = false);
	ExpressionResult interpret(BlockQueue &blocks);

	Value *getLastValue() const;
	Value *&getLastValue();

  private:
	void checkMemory();
	ExpressionResult interpretLine(Line &line, bool clearMemory = true);
	ExpressionResult interpretBlock(Line &line, CodeBlock &block);
	void interpretFString(const FStringToken *token);
	ExpressionResult interpretOperator(const OperatorToken *operatorToken);
	ExpressionResult interpretKeyword(const Token *keywordToken);
	ExpressionResult interpretValueType(const Token *typeToken);
	void interpretAssignment(const Token *operatorToken);
	const RPNFunction *getFunction(const Value *functionName);
	ExpressionResult interpretFunctionCall(Token *functionToken);
	ExpressionResult interpretIf(Line &line, CodeBlock &block);
	ExpressionResult interpretWhile(Line &line, CodeBlock &block);
	ExpressionResult interpretFor(Line &line, CodeBlock &block);
	ExpressionResult interpretTry(Line &line, CodeBlock &block);
	ExpressionResult interpretList(const Token *keywordToken);
	ExpressionResult interpretStruct(const Token *keywordToken);
	ExpressionResult interpretGet(const Token *keywordToken);

	Value *lastValue;
	Memory memory;
	ContextPtr context;
};