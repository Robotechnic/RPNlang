#pragma once

#include <vector>
#include "tokens/token.hpp"
#include "codeblocks/baseblock.hpp"

class Line : public BaseBlock {
	public:
		Line();
		~Line();
		void push(Token *token);
		bool empty() const;
		int size() const;
		Token *pop();
		Token *top();
		void clear();
		void display() const;
		void reset();

		TextRange lastRange() const;

	private:
		std::vector<Token*> tokens;
		long unsigned int currentToken;
};