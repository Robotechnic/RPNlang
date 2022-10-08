#include "codeblocks/codeblock.hpp"

CodeBlock::CodeBlock(Token *keyword) : BaseBlock(CODE_BLOCK), keyword(keyword) {}

CodeBlock::~CodeBlock() {
	this->clear();
	delete this->keyword;
	if (this->next != nullptr) {
		delete this->next;
	}
}

void CodeBlock::push(BaseBlock *block) {
	blocks.push(block);
}

BlockQueue& CodeBlock::getBlocks() {
	return this->blocks;
}

void CodeBlock::setNext(CodeBlock *next) {
	this->next = next;
}

bool CodeBlock::empty() const {
	return this->blocks.empty();
}

void CodeBlock::clear() {
	this->blocks.clear();
}

const Token* CodeBlock::getKeyword() {
	return this->keyword;
}

void CodeBlock::display() const {
	std::cout << "CodeBlock: " << this->keyword->getStringValue() << std::endl;
	this->blocks.display();
	std::cout<< "End CodeBlock: " << this->keyword->getStringValue() << std::endl;
}

TextRange CodeBlock::lastRange() const {
	return this->blocks.lastRange();
}