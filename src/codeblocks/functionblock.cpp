#include "codeblocks/functionblock.hpp"

FunctionBlock::FunctionBlock(
	const std::string &name,
	const std::vector<std::string> &argsName,
	const std::vector<ValueType> &argsTypes,
	const ValueType &returnType,
	CodeBlock *body
) : BaseBlock(FUNCTION_BLOCK) {
	this->function = UserRPNFunction::addFunction(name, argsName, argsTypes, returnType, body);
}

bool FunctionBlock::empty() const {
	return this->function == nullptr;
}

void FunctionBlock::clear() {
	this->function = nullptr;
}

void FunctionBlock::display() const {
	std::cout << "FunctionBlock" << std::endl;
	std::cout << this->function  << std::endl;
	std::cout << "End of FunctionBlock" << std::endl;
}

std::string FunctionBlock::getName() const {
	return this->function->getName();
}

const RPNFunction *FunctionBlock::getFunction() const {
	return this->function.get();
}

TextRange FunctionBlock::lastRange() const {
	return this->function->getRange();
}