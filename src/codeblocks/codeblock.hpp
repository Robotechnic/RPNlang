#pragma once

#include "codeblocks/baseblock.hpp"
#include "codeblocks/blockqueue.hpp"
#include "codeblocks/line.hpp"
#include "tokens/tokens/keywordtoken.hpp"
#include <queue>

class CodeBlock : public BaseBlock {
  public:
	CodeBlock(KeywordToken *keyword);
	~CodeBlock();
	void push(BaseBlock *block);
	BlockQueue &getBlocks();

	void setNext(CodeBlock *next);
	CodeBlock *getNext() const;

	KeywordToken *getKeywordToken();
	KeywordEnum getKeyword() const;
	TextRange getRange() const;

	bool empty() const;
	void clear();
	void reset();
	size_t size() const;

	BlockQueueIterator begin();
	BlockQueueIterator end();

	TextRange lastRange() const;

	void display() const;

  private:
	KeywordToken *keyword;
	CodeBlock *next;
	BlockQueue blocks;
};