#pragma once

#include "tokens/token.hpp"
#include "tokens/tokentypes.hpp"
#include "expressionresult/expressionresult.hpp"
#include "textutilities/escapecharacters.hpp"
#include "codeblocks/blockqueue.hpp"
#include "codeblocks/codeblock.hpp"
#include "codeblocks/line.hpp"
#include "lexer/keywords.hpp"
#include <string>
#include <queue>


class Lexer {
	public:
		Lexer(std::queue<Token> tokens, const Context *context);
		~Lexer();

		ExpressionResult lex();

		BlockQueue& getBlocks() {
			return codeBlocks;
		};

		static ExpressionResult tokenize(int line, std::string lineString, std::queue<Token> &tokens, const Context *context);

	private:
		const Context *context;
		Line *currentLine;
		std::queue<Token> tokens;
		std::stack<CodeBlock*> keywordBlockStack;
		BlockQueue codeBlocks;

		void pushLine();

		ExpressionResult parseBinNumber(Token token);
		ExpressionResult parseHexNumber(Token token);
		ExpressionResult parseFString(Token token);
		ExpressionResult parseString(Token token);
		ExpressionResult parseLiteral(Token token);
		ExpressionResult parseKeyword(Token token);
};