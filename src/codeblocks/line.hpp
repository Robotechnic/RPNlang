#pragma once

#include <vector>
#include "tokens/token.hpp"
#include "codeblocks/baseblock.hpp"

class LineIterator;
class Line : public BaseBlock {
	public:
		Line();
		~Line();
		void push(Token *token);
		bool empty() const;
		int size() const;
		Token *pop();
		Token *top();
		Token *back();
		Token *last();
		void clear();
		void display() const;
		void reset();

		TextRange lastRange() const;
		TextRange lineRange() const;

		LineIterator begin();
		LineIterator end();

	private:
		std::vector<Token*> tokens;
		long unsigned int currentToken;

		friend class LineIterator;
};

class LineIterator {
	public:
		LineIterator(Line *line);
		LineIterator(const LineIterator &other);
		~LineIterator();
		LineIterator &operator=(const LineIterator &other);
		LineIterator &operator++();
		LineIterator operator++(int);
		Token *operator*();
		Token *operator->();
		bool operator==(const LineIterator &other) const;
		bool operator!=(const LineIterator &other) const;
		operator bool() const;

		Line *getLine() const;
		const Line* getLineConst() const;

	private:
		LineIterator(Line *line, long unsigned int currentToken);
		Line *line;
		long unsigned int currentToken;

		friend class Line;
};