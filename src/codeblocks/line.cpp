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

Token *Line::top() {
	return this->tokens[this->currentToken];
}

Token *Line::back() {
	return this->tokens[this->tokens.size() - 1];
}

Token *Line::last() {
	if (this->tokens.empty()) {
		throw std::runtime_error("Line::last() called on empty stack");
	}
	if (this->currentToken == 0) {
		return this->tokens[0];
	}
	return this->tokens[this->currentToken - 1];
}

bool Line::empty() const {
	return this->currentToken >= this->tokens.size();
}

int Line::size() const {
	return this->tokens.size() - this->currentToken;
}

void Line::clear() {
	for (auto token : this->tokens)
		if (token != nullptr) 
			delete token;
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

TextRange Line::lineRange() const {
	if (this->tokens.empty()) {
		throw std::runtime_error("Line::lineRange() called on empty stack");
	}
	return TextRange::merge(this->tokens.front()->getRange(), this->tokens.back()->getRange());
}

LineIterator Line::begin() {
	return LineIterator(this);
}

LineIterator Line::end() {
	return LineIterator(this, this->tokens.size());
}

// Iterator implementation

LineIterator::LineIterator(Line *line) : line(line), currentToken(0) {}
LineIterator::LineIterator(const LineIterator &other) : line(other.line), currentToken(other.currentToken) {}
LineIterator::LineIterator(Line *line, long unsigned int currentToken) : line(line), currentToken(currentToken) {}
LineIterator::~LineIterator() {}

LineIterator &LineIterator::operator=(const LineIterator &other) {
	this->line = other.line;
	this->currentToken = other.currentToken;
	return *this;
}

LineIterator &LineIterator::operator++() {
	this->currentToken++;
	return *this;
}

LineIterator LineIterator::operator++(int) {
	LineIterator tmp(*this);
	operator++();
	return tmp;
}

Token *LineIterator::operator*() {
	return this->line->tokens[this->currentToken];
}

Token *LineIterator::operator->() {
	return this->line->tokens[this->currentToken];
}

bool LineIterator::operator==(const LineIterator &other) const {
	return this->line == other.line && this->currentToken == other.currentToken;
}

bool LineIterator::operator!=(const LineIterator &other) const {
	return !(*this == other);
}

LineIterator::operator bool() const {
	return this->currentToken < this->line->tokens.size();
}

Line *LineIterator::getLine() const {
	return this->line;
}

const Line* LineIterator::getLineConst() const {
	return this->line;
}