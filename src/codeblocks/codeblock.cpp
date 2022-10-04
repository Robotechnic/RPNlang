#include "codeblocks/codeblock.hpp"

CodeBlock::CodeBlock(Token keyword) : BaseBlock(CODE_BLOCK), keyword(keyword) {}

void CodeBlock::push(BaseBlock *block) {
	blocks.push(block);
}

BlockQueue CodeBlock::getBlocks() {
	return this->blocks;
}

bool CodeBlock::empty() const {
	return this->blocks.empty();
}

void CodeBlock::clear() {
	this->blocks.clear();
}

Token CodeBlock::getKeyword() {
	return this->keyword;
}

void CodeBlock::display() const {
	std::cout << "CodeBlock: " << this->keyword.getValue() << std::endl;
	this->blocks.display();
	std::cout<< "End CodeBlock: " << this->keyword.getValue() << std::endl;
}