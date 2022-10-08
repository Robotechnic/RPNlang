#include "codeblocks/line.hpp"

Line::Line() :  BaseBlock(LINE_BLOCK), tokens() {}

Line::~Line() {
	clear();
}

std::queue<Token*> Line::getTokens() {
	return tokens;
}

void Line::push(Token *token) {
	this->tokens.push(token);
}

Token *Line::pop() {
	Token *token = this->tokens.front();
	this->tokens.pop();
	return token;
}

bool Line::empty() const {
	return this->tokens.empty();
}

void Line::clear() {
	while (!this->tokens.empty()) {
		if (this->tokens.front() != nullptr) delete this->tokens.front();
		this->tokens.pop();
	}
}

void Line::display() const {
	std::cout<<"Line: ";
	std::cout<<this->tokens<<";"<<std::endl;
}

TextRange Line::lastRange() const {
	if (this->tokens.empty()) {
		throw std::runtime_error("Line::lastRange() called on empty stack");
	}
	return this->tokens.back()->getRange();
}