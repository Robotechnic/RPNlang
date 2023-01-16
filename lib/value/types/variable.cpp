#include "value/types/variable.hpp"

Variable::Variable(std::string_view value, TextRange range) :
	Value(VARIABLE, range, Value::CONTEXT_VARIABLE),
	value(value) {}


inline std::string Variable::getStringValue() const {
	return value;
}

Value *Variable::to(ValueType type, ValueOwner owner) const {
	throw std::runtime_error("Invalid value type");
}

inline Value *Variable::copy(ValueOwner owner) const {
	return new Variable(value, this->range);
}

operatorResult Variable::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Variable::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"variable object just hold variable name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}
