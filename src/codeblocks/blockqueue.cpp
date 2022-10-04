#include "codeblocks/blockqueue.hpp"
BlockQueue::BlockQueue() : blocks(), lastPopped(nullptr) {}
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
	this->lastPopped = this->blocks.front();
	this->blocks.pop();
	return this->lastPopped;
}

BaseBlock *BlockQueue::lastPop() {
	return this->lastPopped;
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