#pragma once

#include <vector>
#include <ostream>
#include "textutilities/textutilities.hpp"

class TextRange {
	public:
		TextRange();
		TextRange(unsigned long line, unsigned long column, unsigned long length);
		TextRange(const TextRange &other);
		TextRange(TextRange &&other);

		std::string getLine(std::string_view code) const;
		
		TextRange merge(const TextRange &other);
		bool isEmpty() const;

		static TextRange merge(const TextRange left, const TextRange right);

		TextRange& operator=(const TextRange &other);
		TextRange& operator=(TextRange &&other);

		unsigned long line = 0;
		unsigned long columnStart = 0;
		unsigned long columnEnd = 0;
};

bool operator==(const TextRange &left, const TextRange &right);
std::ostream &operator<<(std::ostream &os, const TextRange &range);