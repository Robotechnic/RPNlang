#include "codeblocks/line.hpp"

Line::Line() :  BaseBlock(LINE_BLOCK), tokens(), currentToken(0) {}

Line::~Line() {
	clear();
}

void Line::push(Token *token) {
	this->tokens.push_back(token);
}

Token *Line::pop() {
	return this->tokens[this->currentToken++];
}

bool Line::empty() const {
	return this->currentToken >= this->tokens.size();
}

void Line::clear() {
	for (auto token : this->tokens)
		if (token != nullptr) delete token;
	this->tokens.clear();
}

void Line::display() const {
	std::cout<<"Line: ";
	std::cout<<this->tokens<<";"<<std::endl;
}

/**
 * @brief reset the line to the beginning
 * 
 */
void Line::reset() {
	this->currentToken = 0;
}

TextRange Line::lastRange() const {
	if (this->tokens.empty()) {
		throw std::runtime_error("Line::lastRange() called on empty stack");
	}
	return this->tokens.back()->getRange();
}