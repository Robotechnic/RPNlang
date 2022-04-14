#include "interpreter/expressionresult.hpp"


ExpressionResult::ExpressionResult(Value result) : isError(false), result(result) {}

ExpressionResult::ExpressionResult(std::string errorMessage, int line, int column) :
	isError(true),
	errorMessage(errorMessage),
	line(line),
	column(column)
{
}

bool ExpressionResult::error() const {
	return this->isError;
}

bool ExpressionResult::success() const {
	return !this->isError;
}