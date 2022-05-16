#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "textutilities/textrange.hpp"

class Context;
#include "context/context.hpp"

class ExpressionResult {
	public:
		ExpressionResult();
		ExpressionResult(std::string errorMessage, TextRange errorRange, const Context &context);
		~ExpressionResult();

		bool error() const;
		bool success() const;
		std::string getErrorMessage() const;
		TextRange getRange() const;
		const Context& getContext() const;

		void setRange(const TextRange &range);

		void display(std::string line) const;
		void display (std::ifstream &file) const;

	private:
		bool isError;
		std::string errorMessage;
		TextRange errorRange;
		Context context;
};