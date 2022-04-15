#pragma once

#include <iostream>
#include <string>
#include "textrange/textrange.hpp"

class ExpressionResult {
	public:
		ExpressionResult();
		ExpressionResult(std::string errorMessage, TextRange errorRange, std::string erroredLine = "");

		ExpressionResult setErrorredLine(std::string erroredLine);

		bool error() const;
		bool success() const;
		std::string getErrorMessage() const;
		TextRange getRange() const;
		std::string getErroredLine() const;

	private:
		bool isError;
		std::string errorMessage, erroredLine;
		TextRange errorRange;
};

std::ostream &operator<<(std::ostream &os, const ExpressionResult &result);