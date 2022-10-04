#pragma once 

#include <queue>
#include "tokens/token.hpp"
#include "codeblocks/baseblock.hpp"
#include "codeblocks/blockqueue.hpp"
#include "codeblocks/line.hpp"

class CodeBlock : public BaseBlock {
	public:
		CodeBlock(Token keyword);
		~CodeBlock(){};
		void push(BaseBlock *block);
		BlockQueue getBlocks();

		Token getKeyword();

		bool empty() const;
		void clear();

		void display() const;

	private:
		Token keyword;
		BlockQueue blocks;
};