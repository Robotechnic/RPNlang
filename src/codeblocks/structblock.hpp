#pragma once

#include "value/types/struct.hpp"
#include "codeblocks/baseblock.hpp"

class StructBlock : public BaseBlock {
	public:
		StructBlock(const StructDefinition &structDef);

		bool empty() const override;
		void clear() override;
		void display() const override;
		TextRange lastRange() const override;

		StructDefinition& getDefinition();

	private:
		StructDefinition def;
};