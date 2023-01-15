#pragma once

#include <unordered_map>
#include <memory>
#include "rpnfunctions/rpnfunction.hpp"
#include "codeblocks/codeblock.hpp"
#include "interpreter/interpreter.hpp"

class UserRPNFunction : public RPNFunction {
	public:
		UserRPNFunction();
		UserRPNFunction(const UserRPNFunction &other);
		UserRPNFunction(
			const std::string &name,
			const std::vector<std::string> &argsName,
			const RPNFunctionArgTypes &argsTypes,
			const RPNFunctionValueType &returnType,
			CodeBlock *body
		);
		~UserRPNFunction() override;

		RPNFunctionResult call(
			RPNFunctionArgs &args,
			const TextRange &range,
			ContextPtr context
		) const override;

		static std::shared_ptr<UserRPNFunction> addFunction(
			const std::string &name,
			const std::vector<std::string> &argsName,
			const RPNFunctionArgTypes &argsTypes,
			RPNFunctionValueType returnType, 
			CodeBlock *body
		);

		TextRange getRange() const;

		static std::shared_ptr<UserRPNFunction> getFunction(const std::string &name);

	private:
		void addParameters(RPNFunctionArgs &args, const ContextPtr &context) const;
		CodeBlock *body;

		static std::unordered_map<std::string, std::shared_ptr<UserRPNFunction>> userFunctions;
};