#pragma once

#include <stack>
#include <stdexcept>
#include "codeblocks/baseblock.hpp"

class BlockQueue {
	public:
		BlockQueue();
		~BlockQueue();

		void push(BaseBlock *block);
		BaseBlock *pop();
		BaseBlock *front();
		BaseBlock *back();

		bool empty() const;
		void clear();

		void display() const;

	private:
		std::queue<BaseBlock *> blocks;
};
