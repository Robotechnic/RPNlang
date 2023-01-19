#pragma once

#include "codeblocks/baseblock.hpp"
#include "codeblocks/codeblock.hpp"
#include "rpnfunctions/userrpnfunction.hpp"

class UserRPNFunction;

class FunctionBlock : public BaseBlock {
	public:
		FunctionBlock(
			const std::string &name,
			const RPNFunctionArgs arguments,
			const RPNValueType &returnType,
			CodeBlock *body
		);

		bool empty() const;
		void clear();
		void display() const;
		BlockQueue& getBlocks();

		std::string getName() const;
		const RPNFunction *getFunction() const;

		RPNValueType getReturnType() const;
		RPNFunctionArgs getArgs() const;
		CodeBlock *getBody() const;

		TextRange lastRange() const;
	
	private:
		std::shared_ptr<UserRPNFunction> function;
};