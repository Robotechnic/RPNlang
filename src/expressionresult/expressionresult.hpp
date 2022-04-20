#pragma once

#include <iostream>
#include <string>
#include "textutilities/textrange.hpp"

class ExpressionResult {
	public:
		ExpressionResult();
		ExpressionResult(std::string errorMessage, TextRange errorRange);
		~ExpressionResult();

		bool error() const;
		bool success() const;
		std::string getErrorMessage() const;
		TextRange getRange() const;

		void display(std::string code);
		static void display(ExpressionResult result, std::string code);

	private:
		bool isError;
		std::string errorMessage;
		TextRange errorRange;
};