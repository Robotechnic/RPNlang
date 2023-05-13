#include "codeblocks/codeblock.hpp"

CodeBlock::CodeBlock(KeywordToken *keyword)
	: BaseBlock(CODE_BLOCK), keyword(keyword), next(nullptr) {}

CodeBlock::~CodeBlock() {
	this->clear();
	delete this->keyword;
	if (this->next != nullptr)
		delete this->next;
}

void CodeBlock::push(BaseBlock *block) {
	blocks.push(block);
}

BlockQueue &CodeBlock::getBlocks() {
	return this->blocks;
}

TextRange CodeBlock::getRange() const {
	return this->keyword->getRange();
}

void CodeBlock::setNext(CodeBlock *next) {
	if (this->next != nullptr)
		this->next->setNext(next);
	else
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

size_t CodeBlock::size() const {
	return this->blocks.size();
}

BlockQueueIterator CodeBlock::begin() {
	return this->blocks.begin();
}
BlockQueueIterator CodeBlock::end() {
	return this->blocks.end();
}

KeywordToken *CodeBlock::getKeywordToken() {
	return this->keyword;
}

KeywordEnum CodeBlock::getKeyword() const {
	return this->keyword->getKeyword();
}

void CodeBlock::display() const {
	const std::string name = this->keyword->getStringValue();
	std::cout << "CodeBlock: " << name << std::endl;
	this->blocks.display();
	if (this->next != nullptr)
		this->next->display();
	std::cout << "End CodeBlock: " << name << std::endl;
}

TextRange CodeBlock::lastRange() const {
	return this->blocks.lastRange();
}