#include "codeblocks/baseblock.hpp"

BaseBlock::BaseBlock(blockType type) : type(type) {}

blockType BaseBlock::getType() const {
	return type;
}