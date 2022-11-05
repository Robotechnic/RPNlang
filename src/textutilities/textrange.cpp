#include "textutilities/textrange.hpp"

TextRange::TextRange() : line(0), columnStart(0), columnEnd(0) {}
TextRange::TextRange(int line, int column, int length) : line(line), columnStart(column), columnEnd(column + length) {}
TextRange::TextRange(const TextRange &other) : line(other.line), columnStart(other.columnStart), columnEnd(other.columnEnd) {}
/**
 * @brief split the text in lines and get the line which the range points to
 * 
 * @param code the text to split
 * @return std::string the line
 */
std::string TextRange::getLine(std::string code) {
	std::vector<std::string> lines = split(code, '\n');
	if (line >= lines.size())
		return "";
	return lines[line];
}

/**
 * @brief merge two ranges to get a range which contains both
 * 
 * @param other the other range
 * @return TextRange the merged range
 */
TextRange TextRange::merge(const TextRange &other) {
	if (other.line > this->line)
		line = other.line;
	if (other.columnStart < this->columnStart)
		this->columnStart = other.columnStart;
	if (other.columnEnd > this->columnEnd)
		this->columnEnd = other.columnEnd;
	
	return *this;
}

/**
 * @brief check if the text range is empty
 * 
 * @return bool true if the range is empty
 */
bool TextRange::isEmpty() {
	return this->line == 0 && this->columnStart == 0 && this->columnEnd == 0;
}

TextRange TextRange::merge(const TextRange left, const TextRange right) {
	return TextRange(left).merge(right);
}

/**
 * @brief display the range to the output stream
 * 
 * @param os current output stream
 * @param range text range to display
 * @return std::ostream& new output stream
 */
std::ostream &operator<<(std::ostream &os, const TextRange &range) {
	os << "Line " << range.line << " : " << range.columnStart << " -> " << range.columnEnd;
	return os;
}

