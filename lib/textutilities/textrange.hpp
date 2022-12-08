#pragma once

#include <vector>
#include <ostream>
#include "textutilities/textutilities.hpp"

class TextRange {
	public:
		TextRange();
		TextRange(unsigned int line, unsigned int column, unsigned int length);
		TextRange(const TextRange &other);
		TextRange(TextRange &&other);

		std::string getLine(std::string_view code);
		
		TextRange merge(const TextRange &other);
		bool isEmpty();

		static TextRange merge(const TextRange left, const TextRange right);

		void operator=(const TextRange &other);
		void operator=(TextRange &&other);

		unsigned int line, columnStart, columnEnd;
};

std::ostream &operator<<(std::ostream &os, const TextRange &range);