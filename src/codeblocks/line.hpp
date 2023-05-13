#pragma once

#include "codeblocks/baseblock.hpp"
#include "tokens/token.hpp"
#include <vector>

class LineIterator;
class Line : public BaseBlock {
  public:
	Line();
	~Line() override;
	void push(Token *token);
	bool empty() const override;
	size_t size() const;
	size_t totalSize() const;
	Token *pop();
	Token *top();
	Token *back();
	Token *last();
	void goBack();
	void clear() override;
	void display() const override;
	void reset();

	TextRange lastRange() const override;
	TextRange lineRange() const;

	LineIterator begin();
	LineIterator end();

  private:
	std::vector<Token *> tokens;
	long unsigned int currentToken = 0;

	friend class LineIterator;
};

class LineIterator {
  public:
	explicit LineIterator(Line *line);
	~LineIterator();
	LineIterator &operator++();
	LineIterator operator++(int);
	Token *operator*();
	Token *operator->();
	bool operator==(const LineIterator &other) const;
	bool operator!=(const LineIterator &other) const;
	operator bool() const;

	Line *getLine() const;
	const Line *getLineConst() const;

  private:
	LineIterator(Line *line, long unsigned int currentToken);
	Line *line;
	long unsigned int currentToken;

	friend class Line;
};