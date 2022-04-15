#pragma once

class Interpreter;

#include <string>
#include <vector>
#include <map>
#include "value/value.hpp"
#include "value/valuetype.hpp"
#include "tokens/token.hpp"
#include "expressionresult/expressionresult.hpp"
#include "interpreter/interpreter.hpp"

class RPNFunction {
	public:
		RPNFunction(
			std::string name,
			std::vector<std::string> parameters,
			std::vector<ValueType> parameterTypes,
			ValueType returnType, 
			std::string body
		);

		ExpressionResult tokenize();

		ExpressionResult call(
			std::vector<Value> args,
			std::map<std::string, Value> variables,
			std::map<std::string, RPNFunction> functions
		);

	private:
		ExpressionResult checkArgs(const std::vector<Value> &args);
		ExpressionResult checkTypes(const std::vector<Value> &args);
		void addParameters(const std::vector<Value> &args, std::map<std::string, Value> &variables);

		std::string name;
		std::vector<std::string> parameters;
		std::vector<ValueType> parameterTypes;
		ValueType returnType;
		std::string body;
		std::vector<Token> tokens;
};

std::ostream& operator<<(std::ostream& os, const RPNFunction& function);