#include "codeblocks/blockqueue.hpp"
BlockQueue::BlockQueue() : blocks(), currentBlock(0) {}
BlockQueue::~BlockQueue() {
	this->clear();
}

void BlockQueue::push(BaseBlock *block) {
	this->blocks.push_back(block);
}

BaseBlock *BlockQueue::pop() {
	if (this->empty()) {
		throw std::runtime_error("BlockQueue::pop() called on empty stack");
	}
	return this->blocks[this->currentBlock++];
}

BaseBlock *BlockQueue::front() {
	return this->blocks.at(this->currentBlock);
}

BaseBlock *BlockQueue::back() {
	return this->blocks.back();
}

bool BlockQueue::empty() const {
	return this->currentBlock >= this->blocks.size();
}

int BlockQueue::size() const {
	return this->blocks.size() - this->currentBlock;
}

void BlockQueue::clear() {
	for (auto block : this->blocks)
		if (block != nullptr) delete block;
	this->blocks.clear();
}

void BlockQueue::reset() {
	this->currentBlock = 0;
}

void BlockQueue::display() const {
	std::cout<<"BlockQueue: ";
	for (auto block : this->blocks) {
		block->display();
	}
	std::cout<<std::endl;
}

TextRange BlockQueue::lastRange() const {
	if (this->blocks.empty()) {
		throw std::runtime_error("BlockQueue::lastRange() called on empty stack");
	}
	return this->blocks.back()->lastRange();
}