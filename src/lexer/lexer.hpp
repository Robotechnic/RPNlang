#pragma once

#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <tuple>

#include "analyzer/analyzer.hpp"
#include "codeblocks/blockqueue.hpp"
#include "codeblocks/codeblock.hpp"
#include "codeblocks/functionblock.hpp"
#include "codeblocks/line.hpp"
#include "expressionresult/expressionresult.hpp"
#include "textutilities/escapecharacters.hpp"
#include "tokens/keywords.hpp"
#include "tokens/token.hpp"
#include "tokens/tokens/fstringtoken.hpp"
#include "tokens/tokens/operatortoken.hpp"
#include "tokens/tokens/stringtoken.hpp"
#include "tokens/tokens/valuetoken.hpp"
#include "tokens/tokentypes.hpp"
#include "value/value.hpp"
#include "value/valuetypes.hpp"

class FunctionBlock;

class Lexer {
  public:
	Lexer(const std::deque<Token *> &tokens, ContextPtr context);
	~Lexer();

	ExpressionResult lex();

	BlockQueue &getBlocks();

	static ExpressionResult tokenize(int lineNumber, std::string_view lineString,
									 std::deque<Token *> &tokens, const ContextPtr &context);

  private:
	void pushLine();
	bool hasParentKeywordBlock(const std::vector<KeywordEnum> &keywords) const;
	ExpressionResult parseBinNumber(Token const *token);
	ExpressionResult parseHexNumber(Token const *token);
	ExpressionResult parseFString(Token const *token);
	ExpressionResult parseString(Token const *token);
	ExpressionResult parseLiteral(Token *token);
	ExpressionResult parsePath(Token const *token);
	ExpressionResult parseStructAccess(Token *token);
	ExpressionResult parseKeyword(Token *token);
	std::pair<ExpressionResult, FunctionBlock *> parseFunction(CodeBlock *block);
	ExpressionResult parseStruct(CodeBlock *block);
	ExpressionResult parseFunctionCall(const Token *token);
	ExpressionResult parseType(Token const *token);

  private:
	ContextPtr context;
	Line *currentLine = new Line();
	std::deque<Token *> tokens;
	std::stack<CodeBlock *> keywordBlockStack;
	BlockQueue codeBlocks;
	/**
	 * @brief this member allow to track if the last token was used directly by the lexer so,
	 * it allow to know it is necessary to clean the memory after token usage
	 */
	bool integrated{};
};
