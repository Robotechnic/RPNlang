#pragma once

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
			std::vector<Token> body
		);

		RPNFunctionResult call(
			RPNFunctionArgs args,
			std::map<std::string, Value> variables,
			std::map<std::string, RPNFunction*> functions
		) const;

	private:
		void addParameters(const RPNFunctionArgs &args, std::map<std::string, Value> &variables) const;
		std::vector<Token> body;
};