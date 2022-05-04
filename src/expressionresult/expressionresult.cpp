#include "expressionresult.hpp"


ExpressionResult::ExpressionResult() : isError(false) {}

ExpressionResult::ExpressionResult(std::string errorMessage, TextRange errorRange, const Context &context) :
	isError(true),
	errorMessage(errorMessage),
	errorRange(errorRange),
	context(context)
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
void ExpressionResult::display(std::string code) const {
	std::cout<<this->context<<std::endl;
	
	TextRange range = this->getRange();
	std::cout << "Error : " << this->errorMessage<<std::endl;

	if (this->context.getType() == CONTEXT_TYPE_DEFAULT) {
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
}

/**
 * @brief display error message to the output stream
 * 
 * @param file the file which contains the errored code

 */
void ExpressionResult::display(std::ifstream &file) const {
	std::cout<<this->context<<std::endl;
	TextRange range = this->getRange();
	std::cout << "Error : " << this->errorMessage<<std::endl;

	std::cout << "At line "<< range.line << " and column " << range.columnStart << " :" << std::endl;
	std::string line;
	file.seekg(0, std::ios::beg);
	while (std::getline(file, line)) {}

	std::cout<<line<<std::endl;

	for (long unsigned int i = 0; i <= range.columnEnd; i++) {
			if (i < range.columnStart) {
				std::cout << " ";
			} else {
				std::cout << "^";
			}
		}
		std::cout << std::endl;
}

const Context& ExpressionResult::getContext() const {
	return this->context;
}