#include "codeblocks/functionblock.hpp"

FunctionBlock::FunctionBlock(const std::string &name, const RPNFunctionArgs &arguments,
							 const RPNValueType &returnType, CodeBlock *body)
	: BaseBlock(blockType::FUNCTION_BLOCK) {
	this->function = UserRPNFunction::addFunction(name, arguments, returnType, body);
}

bool FunctionBlock::empty() const {
	return this->function == nullptr;
}

void FunctionBlock::clear() {
	this->function = nullptr;
}

void FunctionBlock::display() const {
	std::cout << "FunctionBlock (" << function->getName() << ")" << std::endl;
	this->function->getBody()->display();
	std::cout << "End of FunctionBlock" << std::endl;
}

BlockQueue &FunctionBlock::getBlocks() {
	return this->function->getBody()->getBlocks();
}

std::string FunctionBlock::getName() const {
	return this->function->getName();
}

const UserRPNFunction *FunctionBlock::getFunction() const {
	return this->function.get();
}

TextRange FunctionBlock::lastRange() const {
	return this->function->getRange();
}

RPNValueType FunctionBlock::getReturnType() const {
	return this->function->getReturnType();
}

RPNFunctionArgs FunctionBlock::getArgs() const {
	return this->function->getArgs();
}

CodeBlock *FunctionBlock::getBody() const {
	return this->function->getBody();
}

FunctionSignature FunctionBlock::getSignature() const {
	return this->function->getSignature();
}
