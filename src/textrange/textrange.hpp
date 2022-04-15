#pragma once

struct TextRange {
	TextRange(){}
	TextRange(int line, int column, int length) {
		this->line = line;
		this->columnStart = column;
		this->columnEnd = column + length;
	}
	int line, columnStart, columnEnd;
};