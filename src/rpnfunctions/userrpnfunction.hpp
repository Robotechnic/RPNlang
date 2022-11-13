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
			const std::vector<ValueType> &argsTypes,
			const ValueType &returnType,
			CodeBlock *body
		);
		~UserRPNFunction() override;

		RPNFunctionResult call(
			const RPNFunctionArgs &args,
			const TextRange &range,
			Context *context
		) const override;

		static std::shared_ptr<UserRPNFunction> addFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argsTypes,
			ValueType returnType, 
			CodeBlock *body
		);

		TextRange getRange() const;

		static std::shared_ptr<UserRPNFunction> getFunction(std::string name);

	private:
		void addParameters(const RPNFunctionArgs &args, Context *context) const;
		CodeBlock *body;

		static std::unordered_map<std::string, std::shared_ptr<UserRPNFunction>> userFunctions;
};