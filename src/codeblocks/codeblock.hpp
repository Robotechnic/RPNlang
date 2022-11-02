#pragma once 

#include <queue>
#include "tokens/token.hpp"
#include "codeblocks/baseblock.hpp"
#include "codeblocks/blockqueue.hpp"
#include "codeblocks/line.hpp"

class CodeBlock : public BaseBlock {
	public:
		CodeBlock(Token *keyword);
		~CodeBlock();
		void push(BaseBlock *block);
		BlockQueue& getBlocks();
		BlockQueue  getBlocksCopy() const;

		void setNext(CodeBlock *next);
		CodeBlock *getNext() const;

		Token* getKeyword();

		bool empty() const;
		void clear();
		void reset();

		TextRange lastRange() const;

		void display() const;

	private:
		Token *keyword;
		CodeBlock *next;
		BlockQueue blocks;
};