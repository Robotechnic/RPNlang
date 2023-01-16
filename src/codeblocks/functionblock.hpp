#pragma once

#include "codeblocks/baseblock.hpp"
#include "codeblocks/codeblock.hpp"
#include "rpnfunctions/userrpnfunction.hpp"

class UserRPNFunction;

class FunctionBlock : public BaseBlock {
	public:
		FunctionBlock(
			const std::string &name,
			const std::vector<std::string> &argsName,
			const RPNFunctionArgTypes &argsTypes,
			const RPNValueType &returnType,
			CodeBlock *body
		);

		bool empty() const;
		void clear();
		void display() const;

		std::string getName() const;
		const RPNFunction *getFunction() const;

		RPNValueType getReturnType() const;
		RPNFunctionArgTypes getArgsTypes() const;
		CodeBlock *getBody() const;

		TextRange lastRange() const;
	
	private:
		std::shared_ptr<UserRPNFunction> function;
};