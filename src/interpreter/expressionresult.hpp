#pragma once

#include <string>
#include "interpreter/value.hpp"

class ExpressionResult {
	public:
		ExpressionResult(Value result);
		ExpressionResult(std::string errorMessage, int line, int column);

		bool error() const;
		bool success() const;

	private:
		bool isError;
		Value result;
		std::string errorMessage;
		int line;
		int column;
};