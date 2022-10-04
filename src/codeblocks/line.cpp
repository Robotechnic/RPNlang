#include "codeblocks/line.hpp"

Line::Line() :  BaseBlock(LINE_BLOCK), tokens() {}

std::queue<Token> Line::getTokens() {
	return tokens;
}

void Line::push(Token token) {
	this->tokens.push(token);
}

bool Line::empty() const {
	return this->tokens.empty();
}

void Line::clear() {
	while (!this->tokens.empty()) {
		this->tokens.pop();
	}
}

void Line::display() const {
	std::queue<Token> tmp = this->tokens;
	std::cout<<"Line: ";
	while (!tmp.empty()) {
		std::cout<<tmp.front() << " ";
		tmp.pop();
	}
	std::cout<<";"<<std::endl;
}