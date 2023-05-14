#include "codeblocks/line.hpp"

Line::Line() : BaseBlock(blockType::LINE_BLOCK) {}

Line::~Line() {
	clear();
}

void Line::push(Token *token) {
	this->tokens.push_back(token);
}

Token *Line::pop() {
	Token *token = this->tokens[this->currentToken];
	this->currentToken++;
	return token;
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

void Line::goBack() {
	if (this->currentToken == 0) {
		throw std::runtime_error("Line::goBack() called on empty stack");
	}
	this->currentToken--;
}

bool Line::empty() const {
	return this->currentToken >= this->tokens.size();
}

size_t Line::size() const {
	return this->tokens.size() - this->currentToken;
}

size_t Line::totalSize() const {
	return this->tokens.size();
}

void Line::clear() {
	for (auto *token : this->tokens) {
		delete token;
	}
	this->tokens.clear();
}

void Line::display() const {
	std::cout << "Line: ";
	std::cout << this->tokens << ";" << std::endl;
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
	return LineIterator{this};
}

LineIterator Line::end() {
	return {this, this->tokens.size()};
}

// Iterator implementation

LineIterator::LineIterator(Line *line) : line(line), currentToken(0) {}
LineIterator::LineIterator(Line *line, long unsigned int currentToken)
	: line(line), currentToken(currentToken) {}
LineIterator::~LineIterator() = default;

LineIterator &LineIterator::operator++() {
	this->currentToken++;
	return *this;
}

LineIterator LineIterator::operator++(int) {
	LineIterator const tmp(*this);
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

const Line *LineIterator::getLineConst() const {
	return this->line;
}
