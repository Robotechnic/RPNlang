#pragma once

#include "codeblocks/baseblock.hpp"
#include <stdexcept>
#include <vector>

class BlockQueueIterator;

class BlockQueue {
  public:
	BlockQueue();
	~BlockQueue();

	void push(BaseBlock *block);
	BaseBlock *pop();
	BaseBlock *popBack();
	BaseBlock *front();
	BaseBlock *back();

	bool empty() const;
	size_t size() const;
	void clear();
	void reset();

	void display() const;

	TextRange lastRange() const;

	BlockQueueIterator begin();
	BlockQueueIterator end();

  private:
	std::vector<BaseBlock *> blocks;
	long unsigned int currentBlock = 0;

	friend BlockQueueIterator;
};

class BlockQueueIterator {
  public:
	BlockQueueIterator(BlockQueue *queue, long unsigned int index);
	BlockQueueIterator(const BlockQueueIterator &it);
	~BlockQueueIterator();

	BlockQueueIterator &operator=(const BlockQueueIterator &it);
	BlockQueueIterator &operator++();
	BlockQueueIterator operator++(int);
	BlockQueueIterator operator+(long unsigned int n);
	BlockQueueIterator operator-(long unsigned int n);
	bool operator<(const BlockQueueIterator &it) const;
	bool operator>(const BlockQueueIterator &it) const;
	bool operator==(const BlockQueueIterator &it) const;
	bool operator!=(const BlockQueueIterator &it) const;
	BaseBlock *operator*() const;
	BaseBlock *operator->() const;
	operator bool() const;

	BlockQueue *getQueue() const;
	const BlockQueue *getConstQueue() const;

  private:
	BlockQueue *queue;
	long unsigned int currentBlock;
};