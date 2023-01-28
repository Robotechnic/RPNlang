#include "expressionresult.hpp"


ExpressionResult::ExpressionResult() : 
	resultStatus(SUCCESS),
	errorMessage(""),
	errorRange(TextRange())
{}

ExpressionResult::ExpressionResult(std::string_view errorMessage, TextRange errorRange, ContextPtr parentContext) :
	resultStatus(ERROR),
	errorMessage(errorMessage),
	errorRange(errorRange),
	context(parentContext)
{}

ExpressionResult::ExpressionResult(Status status) :
	resultStatus(status)
{}

ExpressionResult::ExpressionResult(const ExpressionResult &other) noexcept :
	resultStatus(other.resultStatus),
	errorMessage(other.errorMessage),
	errorRange(other.errorRange),
	context(other.context)
{}

ExpressionResult::ExpressionResult(ExpressionResult &&other) noexcept :
	resultStatus(other.resultStatus),
	errorMessage(other.errorMessage),
	errorRange(other.errorRange),
	context(other.context)
{}

void ExpressionResult::operator=(const ExpressionResult &other) noexcept {
	this->resultStatus = other.resultStatus;
	this->errorMessage = other.errorMessage;
	this->errorRange = other.errorRange;
	this->context = other.context;
}

void ExpressionResult::operator=(ExpressionResult &&other) noexcept {
	this->resultStatus = other.resultStatus;
	this->errorMessage = other.errorMessage;
	this->errorRange = other.errorRange;
	this->context = other.context;
}

/**
 * @brief get if the result is an error
 * 
 * @return true if the result is an error
 */
bool ExpressionResult::error() const {
	return this->resultStatus == ERROR;
}

bool ExpressionResult::breakingLoop() const {
	return this->resultStatus == BREAK;
}

bool ExpressionResult::continuingLoop() const {
	return this->resultStatus == CONTINUE;
}

bool ExpressionResult::returnValue() const {
	return this->resultStatus == RETURN;
}

bool ExpressionResult::stopInterpret() const {
	return this->resultStatus != SUCCESS;
}

/**
 * @brief get if the result is not an error
 * 
 * @return true if the result is not an error
 */
bool ExpressionResult::success() const {
	return this->resultStatus == SUCCESS;
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
void ExpressionResult::displayLineError(std::string_view code) const {
	std::cout<<std::endl;
	std::cout<<this->context;
	
	TextRange range = this->getRange();
	std::cout << "Error : " << this->errorMessage<<std::endl;

	if (this->context->getType() == CONTEXT_TYPE_DEFAULT) {
		std::cout << "At line "<< range.line << " and column " << range.columnStart << " :" << std::endl;
		std::cout << code << std::endl;
		this->displayArrow(range, code);
	}
}

/**
 * @brief display error message to the output stream
 * 
 * @param fileName the file which contains the errored code

 */
void ExpressionResult::display() {
	// reset ANSI color
	std::cout << "\033[0m" << std::endl;

	// enable cursor
	std::cout << "\033[?25h";

	std::cout<<this->context;

	TextRange range = this->getRange();
	std::cout << "Error : " << this->errorMessage<<std::endl;

	if (this->context->hasParentType(CONTEXT_TYPE_BUILTIN_MODULE)) {
		while (
			this->context->getType() != CONTEXT_TYPE_FUNCTION &&
			this->context->getType() != CONTEXT_TYPE_FILE &&
			this->context->getType() != CONTEXT_TYPE_DEFAULT
		) {
			this->context = this->context->getParent();
		}
		std::cout << "Invoked from " << this->context->getTypeName() << std::endl;
	}

	std::ifstream file;
	std::string errorMessage;
	if (!openFile(file, this->context->getFilePath(), errorMessage)) {
		std::cout<<"Error: cannot open file "<<this->context->getFilePath()<<": "<<errorMessage<<std::endl;
		return;
	}

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

void ExpressionResult::displayArrow(TextRange range, std::string_view lineString) const {
	for (long unsigned int i = 1; i <= range.columnEnd; i++) {
		if (lineString[i - 1] == '\t') {
			std::cout << "\t";
		} else if (i < range.columnStart) {
			std::cout << " ";
		} else {
			std::cout << "^";
		}
	}
	std::cout << std::endl;
}

ContextPtr ExpressionResult::getContext() const {
	return this->context;
}
