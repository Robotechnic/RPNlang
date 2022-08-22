#include "value/types/variable.hpp"

Variable::Variable(ValueType type, std::string value, TextRange range) :
	Value(type, range),
	value(value) {}


bool Variable::isCastableTo(ValueType type) const {
	return type == VARIABLE;
}

std::string Variable::getStringValue() const {
	return value;
}

Value *Variable::to(ValueType type) {
	throw std::runtime_error("Invalid value type");
}

Value *Variable::copy() const {
	return new Variable(type, value, range);
}

operatorReturn Variable::opadd(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorReturn Variable::opsub(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorReturn Variable::opmul(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorReturn Variable::opdiv(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorReturn Variable::opmod(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorReturn Variable::oppow(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorReturn Variable::opgt(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorReturn Variable::opge(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorReturn Variable::oplt(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorReturn Variable::ople(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorReturn Variable::opne(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorReturn Variable::opeq(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

Variable Variable::emptyVariable = Variable(VARIABLE, "", TextRange());