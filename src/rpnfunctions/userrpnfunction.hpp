#pragma once

#include <map>
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

		static void addFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argsTypes,
			ValueType returnType, 
			CodeBlock *body
		);

		static UserRPNFunction *getFunction(std::string name);

	private:
		void addParameters(const RPNFunctionArgs &args, Context *context) const;
		CodeBlock *body;

		static std::map<std::string, UserRPNFunction> userFunctions;
};