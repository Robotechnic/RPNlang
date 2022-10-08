#include "codeblocks/blockqueue.hpp"
BlockQueue::BlockQueue() : blocks() {}
BlockQueue::~BlockQueue() {
	this->clear();
}

void BlockQueue::push(BaseBlock *block) {
	this->blocks.push(block);
}

BaseBlock *BlockQueue::pop() {
	if (this->blocks.empty()) {
		throw std::runtime_error("BlockQueue::pop() called on empty stack");
	}
	BaseBlock *block = this->blocks.front();
	this->blocks.pop();
	return block;
}

BaseBlock *BlockQueue::front() {
	return this->blocks.front();
}

BaseBlock *BlockQueue::back() {
	return this->blocks.back();
}

bool BlockQueue::empty() const {
	return this->blocks.empty();
}

void BlockQueue::clear() {
	while (!this->blocks.empty()) {
		delete this->blocks.front();
		this->blocks.pop();
	}
}

void BlockQueue::display() const {
	std::queue<BaseBlock *> tmp = this->blocks;
	while (!tmp.empty()) {
		tmp.front()->display();
		tmp.pop();
	}
}

TextRange BlockQueue::lastRange() const {
	if (this->blocks.empty()) {
		throw std::runtime_error("BlockQueue::lastRange() called on empty stack");
	}
	return this->blocks.back()->lastRange();
}