#pragma once

#include "codeblocks/codeblock.hpp"
#include "interpreter/interpreter.hpp"
#include "rpnfunctions/rpnfunction.hpp"
#include <memory>
#include <unordered_map>

class UserRPNFunction : public RPNFunction {
  public:
	UserRPNFunction();
	UserRPNFunction(const UserRPNFunction &other);
	UserRPNFunction(const std::string &name, const RPNFunctionArgs &arguments,
					const RPNValueType &returnType, CodeBlock *body);
	~UserRPNFunction() override;

	RPNFunctionResult call(RPNFunctionArgsValue &args, const TextRange &range,
						   ContextPtr context) const override;

	static std::shared_ptr<UserRPNFunction> addFunction(const std::string &name,
														const RPNFunctionArgs &arguments,
														RPNValueType returnType, CodeBlock *body);

	CodeBlock *getBody() const;

	TextRange getRange() const;

	static std::shared_ptr<UserRPNFunction> getFunction(const std::string &name);

  private:
	void addParameters(RPNFunctionArgs &args, const ContextPtr &context) const;
	CodeBlock *body;

	static std::unordered_map<std::string, std::shared_ptr<UserRPNFunction>> userFunctions;
};