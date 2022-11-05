#pragma once

#include <vector>
#include <ostream>
#include "textutilities/textutilities.hpp"

class TextRange {
	public:
		TextRange();
		TextRange(int line, int column, int length);
		TextRange(const TextRange &other);

		std::string getLine(std::string code);
		
		TextRange merge(const TextRange &other);
		bool isEmpty();

		static TextRange merge(const TextRange left, const TextRange right);

		unsigned long int line, columnStart, columnEnd;
};

std::ostream &operator<<(std::ostream &os, const TextRange &range);