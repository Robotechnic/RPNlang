#pragma once

#include <queue>
#include "rpnfunctions/rpnfunction.hpp"

class Interpreter;

#include "interpreter/interpreter.hpp"


class UserRPNFunction : public RPNFunction {
	public:
		UserRPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> parameterTypes,
			ValueType returnType, 
			std::queue<Token> body
		);

		RPNFunctionResult call(
			RPNFunctionArgs args,
			const Context &context
		) const;

		TextRange getRange() const;

	private:
		void addParameters(const RPNFunctionArgs &args, Context &context) const;
		std::queue<Token> body;
};