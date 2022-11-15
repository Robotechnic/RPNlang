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

BaseBlock *BlockQueue::popBack() {
	if (this->empty()) {
		throw std::runtime_error("BlockQueue::popBack() called on empty stack");
	}
	BaseBlock *back = this->blocks.back();
	this->blocks.pop_back();
	return back;
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

BlockQueueIterator BlockQueue::begin() {
	return BlockQueueIterator(this, 0);
}

BlockQueueIterator BlockQueue::end() {
	return BlockQueueIterator(this, this->blocks.size() - 1);
}


// Iteraror definition
BlockQueueIterator::BlockQueueIterator(BlockQueue *queue, long unsigned int index) : queue(queue), currentBlock(index) {}
BlockQueueIterator::BlockQueueIterator(const BlockQueueIterator &it) : queue(it.queue), currentBlock(it.currentBlock) {}
BlockQueueIterator::~BlockQueueIterator() {}

BlockQueueIterator &BlockQueueIterator::operator=(const BlockQueueIterator &it) {
	this->queue = it.queue;
	this->currentBlock = it.currentBlock;
	return *this;
}

BlockQueueIterator &BlockQueueIterator::operator++() {
	if (this->currentBlock >= this->queue->blocks.size()) {
		throw std::runtime_error("BlockQueueIterator::operator++() called on invalid iterator");
	}
	this->currentBlock++;
	return *this;
}

BlockQueueIterator BlockQueueIterator::operator++(int) {
	BlockQueueIterator it = *this;
	++(*this);
	return it;
}

bool BlockQueueIterator::operator==(const BlockQueueIterator &it) const {
	return this->queue == it.queue && this->currentBlock == it.currentBlock;
}

bool BlockQueueIterator::operator!=(const BlockQueueIterator &it) const {
	return !(*this == it);
}

BaseBlock *BlockQueueIterator::operator*() const {
	return this->queue->blocks.at(this->currentBlock);
}

BaseBlock *BlockQueueIterator::operator->() const {
	return this->queue->blocks.at(this->currentBlock);
}

BlockQueueIterator::operator bool() const {
	return this->currentBlock < this->queue->blocks.size();
}

BlockQueue *BlockQueueIterator::getQueue() const {
	return this->queue;
}

const BlockQueue *BlockQueueIterator::getConstQueue() const {
	return this->queue;
}