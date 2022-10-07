#pragma once

#include <string>
#include <queue>

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

		BlockQueue& getBlocks() {
			return codeBlocks;
		};

		static ExpressionResult tokenize(int line, std::string lineString, std::queue<Token*> &tokens, const Context *context);

	private:
		const Context *context;
		Line *currentLine;
		std::queue<Token*> tokens;
		std::stack<CodeBlock*> keywordBlockStack;
		BlockQueue codeBlocks;

		void pushLine();

		ExpressionResult parseBinNumber(Token *token);
		ExpressionResult parseHexNumber(Token *token);
		ExpressionResult parseFString(Token *token);
		ExpressionResult parseString(Token *token);
		ExpressionResult parseLiteral(Token *token);
		ExpressionResult parseKeyword(Token *token);
		ExpressionResult parseFunctionCall(Token *token);
};