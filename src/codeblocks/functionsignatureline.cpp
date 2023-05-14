#include "codeblocks/functionsignatureline.hpp"

FunctionSignatureLine::FunctionSignatureLine(FunctionSignature signature,
											 const std::string_view &name, const TextRange &range)
	: BaseBlock(blockType::FUNCTION_SIGNATURE), signature(signature), name(name), range(range) {}

FunctionSignature FunctionSignatureLine::getSignature() const {
	return signature;
}

void FunctionSignatureLine::display() const {
	std::cout << "FunctionSignatureLine: \n\t";
	for (const auto &arg : signature.args) {
		std::cout << arg.name() << ", ";
	}
	std::cout << "\n->" << signature.returnType.name() << std::endl;
}

TextRange FunctionSignatureLine::lastRange() const {
	return range;
}

std::string FunctionSignatureLine::getName() const {
	return name;
}
