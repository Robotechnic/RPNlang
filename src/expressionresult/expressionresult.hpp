#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "textutilities/textrange.hpp"
#include "context/context.hpp"

class ExpressionResult {
	public:
		enum Status {
			SUCCESS,
			ERROR,
			BREAK,
			CONTINUE,
			RETURN
		};
	public:
		ExpressionResult();
		ExpressionResult(std::string errorMessage, TextRange errorRange, const Context *context);
		ExpressionResult(Status status);
		~ExpressionResult();

		bool error() const;
		bool success() const;
		std::string getErrorMessage() const;
		TextRange getRange() const;
		const Context *getContext() const;

		void displayLineError(std::string line) const;
		void display () const;

		bool breakingLoop() const;
		bool continuingLoop() const;

		bool stopInterpret() const;

		void operator=(const ExpressionResult &other);

	private:
		void displayArrow(TextRange range, std::string lineString) const;
		Status resultStatus;
		std::string errorMessage;
		TextRange errorRange;
		const Context *context;
};