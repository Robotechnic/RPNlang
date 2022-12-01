#include "value/types/variable.hpp"

Variable::Variable(std::string value, TextRange range) :
	Value(VARIABLE, range, false),
	value(value) {}


bool Variable::isCastableTo(ValueType type) const {
	return type == VARIABLE;
}

std::string Variable::getStringValue() const {
	return value;
}

Value *Variable::to(ValueType type, bool interpreterValue) {
	throw std::runtime_error("Invalid value type");
}

inline Value *Variable::copy(bool interpreterValue) const {
	return new Variable(value, range);
}

operatorResult Variable::opadd(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opsub(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opmul(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opdiv(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opmod(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::oppow(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opgt(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opge(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::oplt(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::ople(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opne(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opeq(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			this->range,
			context	
		),
		nullptr
	);
}
