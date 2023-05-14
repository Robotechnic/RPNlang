#pragma once

#include "codeblocks/baseblock.hpp"
#include "analyzer/functionsignature.hpp"
#include "textutilities/textrange.hpp"

class FunctionSignatureLine : public BaseBlock {
public:
	FunctionSignatureLine(FunctionSignature signature, const std::string_view &name, const TextRange &range);
	~FunctionSignatureLine() override = default;

	FunctionSignature getSignature() const;

	bool empty() const override {
		return false;
	}

	void clear() override {}

	void display() const override;

	TextRange lastRange() const override;

private:
	FunctionSignature signature;
	std::string name;
	TextRange range;
};
