#pragma once

#include "codeblocks/baseblock.hpp"
#include "codeblocks/codeblock.hpp"
#include "rpnfunctions/typedef.hpp"
#include "rpnfunctions/userrpnfunction.hpp"
#include "rpnfunctions/functionsignature.hpp"

class UserRPNFunction;

class FunctionBlock : public BaseBlock {
  public:
	FunctionBlock(const std::string &name, const RPNFunctionArgs &arguments,
				  const RPNValueType &returnType, CodeBlock *body);

	bool empty() const override;
	void clear() override;
	void display() const override;
	BlockQueue &getBlocks();

	std::string getName() const;
	const UserRPNFunction *getFunction() const;

	RPNValueType getReturnType() const;
	RPNFunctionArgs getArgs() const;
	CodeBlock *getBody() const;

	TextRange lastRange() const override;

	FunctionSignature getSignature() const;

  private:
	std::shared_ptr<UserRPNFunction> function;
};
