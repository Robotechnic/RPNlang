#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include "textutilities/textrange.hpp"
#include "context/context.hpp"

class Context;

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
		ExpressionResult(std::string_view errorMessage, TextRange errorRange, ContextPtr parentContext);
		ExpressionResult(std::string_view errorMessage, TextRange &&errorRange, ContextPtr parentContext);
		ExpressionResult(Status status);
		ExpressionResult(const ExpressionResult &other) noexcept;
		ExpressionResult(ExpressionResult &&other) noexcept;
		void operator=(const ExpressionResult &other) noexcept;
		void operator=(ExpressionResult &&other) noexcept;

		bool error() const;
		bool success() const;
		std::string getErrorMessage() const;
		TextRange getRange() const;
		ContextPtr getContext() const;

		void displayLineError(std::string_view line) const;
		void display() const;

		bool breakingLoop() const;
		bool continuingLoop() const;
		bool returnValue() const;

		bool stopInterpret() const;

	private:
		void displayArrow(TextRange range, std::string_view lineString) const;
		Status resultStatus;
		std::string errorMessage;
		TextRange errorRange;
		ContextPtr context;
};