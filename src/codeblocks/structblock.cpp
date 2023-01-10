#include "codeblocks/structblock.hpp"


StructBlock::StructBlock(const StructDefinition &structDef) : BaseBlock(STRUCT_BLOCK), def(structDef) {}


bool StructBlock::empty() const {
	return false;
}

void StructBlock::clear() {
	throw std::runtime_error("Cannot clear StructBlock");
}

void StructBlock::display() const {
	std::cout << "StructBlock" << std::endl;
	std::cout << this->def << std::endl;
	std::cout << "End of StructBlock" << std::endl;
}

TextRange StructBlock::lastRange() const {
	return TextRange();
}

StructDefinition& StructBlock::getDefinition() {
	return this->def;
}