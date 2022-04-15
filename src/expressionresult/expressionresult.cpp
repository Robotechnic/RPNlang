#include "expressionresult.hpp"


ExpressionResult::ExpressionResult() : isError(false) {}

ExpressionResult::ExpressionResult(std::string errorMessage, TextRange errorRange, std::string erroredLine) :
	isError(true),
	errorMessage(errorMessage),
	erroredLine(erroredLine),
	errorRange(errorRange)
{}

ExpressionResult ExpressionResult::setErrorredLine(std::string erroredLine) {
	this->erroredLine = erroredLine;
	return *this;
}

bool ExpressionResult::error() const {
	return this->isError;
}

bool ExpressionResult::success() const {
	return !this->isError;
}

std::string ExpressionResult::getErrorMessage() const {
	return this->errorMessage;
}

TextRange ExpressionResult::getRange() const {
	return this->errorRange;
}

std::string ExpressionResult::getErroredLine() const {
	return this->erroredLine;
}

std::ostream &operator<<(std::ostream &os, const ExpressionResult &result) {
	TextRange range = result.getRange();
	os << "Error : " << result.getErrorMessage()<<std::endl;
	os << "At line "<< range.line << " and column " << range.columnStart << " :" << std::endl;
	os << result.getErroredLine() << std::endl;
	for (int i = 0; i < range.columnStart; i++)
		os << " ";
	for (int i = 0; i < range.columnEnd - range.columnStart; i++)
		os << "~";
	os << std::endl;
	return os;
}