#pragma once

#include "codeblocks/baseblock.hpp"
#include "codeblocks/codeblock.hpp"
#include "rpnfunctions/userrpnfunction.hpp"

class FunctionBlock : public BaseBlock {
	public:
		FunctionBlock(
			const std::string &name,
			const std::vector<std::string> &argsName,
			const std::vector<ValueType> &argsTypes,
			const ValueType &returnType,
			CodeBlock *body
		);

		bool empty() const;
		void clear();
		void display() const;

		std::string getName() const;
		RPNFunction *getFunction() const;

		TextRange lastRange() const;
	
	private:
		RPNFunction *function;
};