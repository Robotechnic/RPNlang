#pragma once

#include <vector>
#include <stdexcept>
#include "codeblocks/baseblock.hpp"

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
		int size() const;
		void clear();
		void reset();

		void display() const;

		TextRange lastRange() const;

		BlockQueueIterator begin();
		BlockQueueIterator end();

	private:
		std::vector<BaseBlock *> blocks;
		long unsigned int currentBlock;
	
	friend BlockQueueIterator;
};

class BlockQueueIterator {
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = BaseBlock;
		using difference_type = std::ptrdiff_t;
		using pointer = BaseBlock *;
		using reference = BaseBlock &;

	public:
		BlockQueueIterator(BlockQueue *queue, long unsigned int index);
		BlockQueueIterator(const BlockQueueIterator &it);
		~BlockQueueIterator();

		BlockQueueIterator &operator=(const BlockQueueIterator &it);
		BlockQueueIterator &operator++();
		BlockQueueIterator operator++(int);
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