#pragma once

#include "rpnfunctions/rpnfunction.hpp"
#include "codeblocks/codeblock.hpp"
#include "interpreter/interpreter.hpp"

class UserRPNFunction : public RPNFunction {
	public:
		UserRPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argsTypes,
			ValueType returnType, 
			CodeBlock *body
		);
		~UserRPNFunction();

		RPNFunctionResult call(
			RPNFunctionArgs args,
			Context *context
		) const;

	private:
		void addParameters(const RPNFunctionArgs &args, Context *context) const;
		CodeBlock *body;
};