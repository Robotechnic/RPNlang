#pragma once

#include <vector>
#include <stdexcept>
#include "codeblocks/baseblock.hpp"

class BlockQueue {
	public:
		BlockQueue();
		~BlockQueue();

		void push(BaseBlock *block);
		BaseBlock *pop();
		BaseBlock *popBack();
		BaseBlock *front();
		BaseBlock *back();

		bool empty() const;
		int size() const;
		void clear();
		void reset();

		void display() const;

		TextRange lastRange() const;
	private:
		std::vector<BaseBlock *> blocks;
		long unsigned int currentBlock;
};
