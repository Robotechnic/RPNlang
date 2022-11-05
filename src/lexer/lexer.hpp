#pragma once

#include <string>
#include <queue>
#include <stack>

#include "tokens/token.hpp"
#include "tokens/tokentypes.hpp"
#include "tokens/tokens/stringtoken.hpp"
#include "tokens/tokens/fstringtoken.hpp"
#include "tokens/tokens/valuetoken.hpp"

#include "expressionresult/expressionresult.hpp"
#include "textutilities/escapecharacters.hpp"
#include "codeblocks/blockqueue.hpp"
#include "codeblocks/codeblock.hpp"
#include "codeblocks/line.hpp"
#include "lexer/keywords.hpp"

#include "value/types/numbers/int.hpp"
#include "value/types/numbers/float.hpp"
#include "value/types/numbers/bool.hpp"
#include "value/types/variable.hpp"



class Lexer {
	public:
		Lexer(std::queue<Token*> tokens, const Context *context);
		~Lexer();

		ExpressionResult lex();

		BlockQueue& getBlocks();

		static ExpressionResult tokenize(int line, std::string lineString, std::queue<Token*> &tokens, const Context *context);

	private:
		void pushLine();
		bool hasParentKeywordBlock(const std::vector<std::string> &keywords) const;
		ExpressionResult parseBinNumber(Token *token);
		ExpressionResult parseHexNumber(Token *token);
		ExpressionResult parseFString(Token *token);
		ExpressionResult parseString(Token *token);
		ExpressionResult parseLiteral(Token *token);
		ExpressionResult parseKeyword(Token *token);
		ExpressionResult parseFunctionCall(Token *token);

	private:
		const Context *context;
		Line *currentLine;
		std::queue<Token*> tokens;
		std::stack<CodeBlock*> keywordBlockStack;
		BlockQueue codeBlocks;
		/**
		 * @brief this member allow to track if the las token was used directly by the lexer so, it allow to know it is 
		 * @brief necessary to clean the memory of the token
		 */
		bool integrated;
};