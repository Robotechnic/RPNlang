#include "rpnfunctions/builtinrpnfunction.hpp"

template <class Signature>
BuiltInRPNFunction<Signature>::BuiltInRPNFunction(
			std::string name,
			std::vector<std::string> parameters,
			std::vector<ValueType> parameterTypes,
			ValueType returnType,
			std::function<Signature> func
		) :
	RPNFunction(name, parameters, parameterTypes, returnType, ""),
	func(func)
{
	if (parameters.size() != parameterTypes.size()) {
		throw std::runtime_error("BuiltInRPNFunction: parameters and parameterTypes must have the same size");
	}
}

template <class Signature>
std::tuple<ExpressionResult, Value> BuiltInRPNFunction<Signature>::call(
	std::vector<Value> args,
	std::map<std::string, Value> variables,
	std::map<std::string, RPNFunction> functions
) {
	ExpressionResult result = this->checkArgs(args);
	if (result.error()) return std::make_tuple(result, Value());



	return std::make_tuple(ExpressionResult(), Value());
}