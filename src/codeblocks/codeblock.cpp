#include "codeblocks/codeblock.hpp"

CodeBlock::CodeBlock(Token *keyword) : BaseBlock(CODE_BLOCK), keyword(keyword), next(nullptr) {}

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

TextRange CodeBlock::getRange() const {
	return this->keyword->getRange();
}

void CodeBlock::setNext(CodeBlock *next) {
	this->next = next;
}

CodeBlock *CodeBlock::getNext() const {
	return this->next;
}

bool CodeBlock::empty() const {
	return this->blocks.empty();
}

void CodeBlock::clear() {
	this->blocks.clear();
}

/**
 * @brief Resets the block queue to the beginning
 * 
 */
void CodeBlock::reset() {
	this->blocks.reset();
}

Token* CodeBlock::getKeyword() {
	return this->keyword;
}

void CodeBlock::display() const {
	const std::string &name = this->keyword->getStringValue();
	std::cout << "CodeBlock: " << name << std::endl;
	this->blocks.display();
	std::cout<< "End CodeBlock: " << name << std::endl;
}

TextRange CodeBlock::lastRange() const {
	return this->blocks.lastRange();
}