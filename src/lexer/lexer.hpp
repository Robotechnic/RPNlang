#pragma once

#include <string>
#include <queue>
#include <stack>
#include <tuple>

#include "analyzer/analyzer.hpp"
#include "expressionresult/expressionresult.hpp"
#include "textutilities/escapecharacters.hpp"
#include "tokens/token.hpp"
#include "tokens/tokentypes.hpp"
#include "tokens/keywords.hpp"
#include "tokens/tokens/stringtoken.hpp"
#include "tokens/tokens/fstringtoken.hpp"
#include "tokens/tokens/valuetoken.hpp"
#include "tokens/tokens/operatortoken.hpp"
#include "value/value.hpp"
#include "value/valuetypes.hpp"
#include "codeblocks/blockqueue.hpp"
#include "codeblocks/codeblock.hpp"
#include "codeblocks/line.hpp"
#include "codeblocks/functionblock.hpp"


class FunctionBlock;

class Lexer {
	public:
		Lexer(std::queue<Token*> tokens, ContextPtr context);
		~Lexer();

		ExpressionResult lex();

		BlockQueue& getBlocks();

		static ExpressionResult tokenize(
			int lineNumber, 
			std::string_view lineString, 
			std::queue<Token*> &tokens,
			const ContextPtr &context
		);

	private:
		void pushLine();
		bool hasParentKeywordBlock(const std::vector<KeywordEnum> &keywords) const;
		ExpressionResult parseBinNumber(Token *token);
		ExpressionResult parseHexNumber(Token *token);
		ExpressionResult parseFString(Token *token);
		ExpressionResult parseString(Token *token);
		ExpressionResult parseLiteral(Token *token);
		ExpressionResult parsePath(Token *token);
		ExpressionResult parseStructAccess(Token *token);
		ExpressionResult parseKeyword(Token *token);
		std::pair<ExpressionResult, FunctionBlock*> parseFunction(CodeBlock *block);
		ExpressionResult parseStruct(CodeBlock *block);
		ExpressionResult parseFunctionCall(const Token *token);
		ExpressionResult parseType(const Token *token);

	private:
		ContextPtr context;
		Line *currentLine;
		std::queue<Token*> tokens;
		std::stack<CodeBlock*> keywordBlockStack;
		BlockQueue codeBlocks;
		/**
		 * @brief this member allow to track if the last token was used directly by the lexer so,
		 * it allow to know it is necessary to clean the memory after token usage
		 */
		bool integrated;
};
