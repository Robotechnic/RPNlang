#include "expressionresult.hpp"


ExpressionResult::ExpressionResult() : 
	isError(false),
	breakLoop(false),
	continueLoop(false)
{
	this->context = nullptr;
}

ExpressionResult::ExpressionResult(std::string errorMessage, TextRange errorRange, const Context *context) :
	isError(true),
	errorMessage(errorMessage),
	errorRange(errorRange),
	breakLoop(false),
	continueLoop(false),
	context(context)
{}

ExpressionResult::ExpressionResult(bool breakLoop, bool continueLoop) :
	isError(false),
	breakLoop(breakLoop),
	continueLoop(continueLoop)
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

bool ExpressionResult::breakingLoop() const {
	return this->breakLoop;
}

bool ExpressionResult::continuingLoop() const {
	return this->continueLoop;
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

void ExpressionResult::setRange(const TextRange &range) {
	this->errorRange = range;
}

/**
 * @brief display the error message to the output stream
 * 
 * @param code lines of code
 */
void ExpressionResult::displayLineError(std::string code) const {
	std::cout<<std::endl;
	std::cout<<this->context;
	
	TextRange range = this->getRange();
	std::cout << "Error : " << this->errorMessage<<std::endl;

	if (this->context->getType() == CONTEXT_TYPE_DEFAULT) {
		std::cout << "At line "<< range.line << " and column " << range.columnStart << " :" << std::endl;
		std::cout << range.getLine(code) << std::endl;
		this->displayArrow(range, code);
	}
}

/**
 * @brief display error message to the output stream
 * 
 * @param fileName the file which contains the errored code

 */
void ExpressionResult::display(std::string fileName) const {
	std::cout<<std::endl;
	std::ifstream file(fileName);

	std::cout<<this->context;
	TextRange range = this->getRange();
	std::cout << "Error : " << this->errorMessage<<std::endl;
	std::cout << "At line "<< range.line << " and column " << range.columnStart << " :" << std::endl;
	
	std::string lineString;
	file.seekg(0);
	unsigned long int line = 1;
	while (std::getline(file, lineString) && line < range.line) {
		line++;
	}

	std::cout<<lineString<<std::endl;

	this->displayArrow(range, lineString);
}

void ExpressionResult::displayArrow(TextRange range, std::string lineString) const {
	for (long unsigned int i = 0; i <= range.columnEnd; i++) {
		if (i < range.columnStart) {
			if (lineString[i] == '\t') {
				std::cout << "\t";
			} else {
				std::cout << " ";
			}
		} else {
			if (lineString[i] == '\t') {
				std::cout << "^\t";
			} else {
				std::cout << "^";
			}
		}
	}
	std::cout << std::endl;
}

const Context* ExpressionResult::getContext() const {
	return this->context;
}