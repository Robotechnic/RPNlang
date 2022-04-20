#pragma once

#include <vector>
#include <ostream>
#include "textutilities/textutilities.hpp"

class TextRange {
	public:
		TextRange();
		TextRange(int line, int column, int length);

		std::string getLine(std::string code);

		unsigned long int line, columnStart, columnEnd;
};

std::ostream &operator<<(std::ostream &os, const TextRange &range);