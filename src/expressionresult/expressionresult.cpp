#include "expressionresult.hpp"


ExpressionResult::ExpressionResult() : isError(false) {}

ExpressionResult::ExpressionResult(std::string errorMessage, TextRange errorRange) :
	isError(true),
	errorMessage(errorMessage),
	errorRange(errorRange)
{}

ExpressionResult::~ExpressionResult() {
	this->errorMessage.clear();
}

/**
 * @brief get if the result is an error
 * 
 * @return true if the result is an error
 */
bool ExpressionResult::error() const {
	return this->isError;
}

/**
 * @brief get if the result is not an error
 * 
 * @return true if the result is not an error
 */
bool ExpressionResult::success() const {
	return !this->isError;
}

std::string ExpressionResult::getErrorMessage() const {
	return this->errorMessage;
}

TextRange ExpressionResult::getRange() const {
	return this->errorRange;
}

/**
 * @brief display the error message to the output stream
 * 
 * @param code lines of code
 */
void ExpressionResult::display(std::string code) {
	ExpressionResult::display(*this, code);
}

/**
 * @brief display the error message to the output stream
 * 
 * @param result the result to display
 * @param code lines of code
 */
void ExpressionResult::display(ExpressionResult result, std::string code) {
	TextRange range = result.getRange();
	std::cout << "Error : " << result.getErrorMessage()<<std::endl;
	std::cout << "At line "<< range.line << " and column " << range.columnStart << " :" << std::endl;
	std::cout << range.getLine(code) << std::endl;
	for (long unsigned int i = 0; i <= range.columnEnd; i++) {
		if (i < range.columnStart) {
			std::cout << " ";
		} else {
			std::cout << "^";
		}
	}
	std::cout << std::endl;
}