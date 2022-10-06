#pragma once

#include <queue>
#include "tokens/token.hpp"
#include "codeblocks/baseblock.hpp"

class Line : public BaseBlock {
	public:
		Line();
		~Line();
		std::queue<Token*> getTokens();
		void push(Token *token);
		bool empty() const;
		void clear();
		void display() const;

	private:
		std::queue<Token*> tokens;
};