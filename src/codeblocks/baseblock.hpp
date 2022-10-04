#pragma once

#include <queue>
#include <iostream>
#include "codeblocks/blocktypes.hpp"


class BaseBlock {
	public:
		BaseBlock(blockType type);
		virtual ~BaseBlock(){};
		blockType getType() const;
		virtual bool empty() const = 0;
		virtual void clear() = 0;
		virtual void display() const = 0;

	private:
		blockType type;
};

