#include "textutilities/textrange.hpp"

TextRange::TextRange() : line(0), columnStart(0), columnEnd(0) {}
TextRange::TextRange(int line, int column, int length) : line(line), columnStart(column), columnEnd(column + length) {}

std::string TextRange::getLine(std::string code) {
	std::vector<std::string> lines = split(code, '\n');
	if (line >= lines.size())
		return "";
	return lines[line];
}


std::ostream &operator<<(std::ostream &os, const TextRange &range) {
	os << "Line " << range.line << " : " << range.columnStart << " -> " << range.columnEnd;
	return os;
}