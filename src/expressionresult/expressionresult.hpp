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
		ExpressionResult(std::string errorMessage, TextRange errorRange, const Context *context);
		ExpressionResult(bool breakLoop, bool continueLoop);
		~ExpressionResult();

		bool error() const;
		bool success() const;
		std::string getErrorMessage() const;
		TextRange getRange() const;
		const Context *getContext() const;

		void displayLineError(std::string line) const;
		void display (std::string fileName) const;

		bool breakingLoop() const;
		bool continuingLoop() const;

		void operator=(const ExpressionResult &other);

	private:
		void displayArrow(TextRange range, std::string lineString) const;
		bool isError;
		std::string errorMessage;
		TextRange errorRange;
		bool breakLoop, continueLoop;
		const Context *context;
};