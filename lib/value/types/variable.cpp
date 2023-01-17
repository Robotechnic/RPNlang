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

Value *Variable::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("variable object just hold variable name in memory, replace it by another value to use it");
}

Value *Variable::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("variable object just hold variable name in memory, replace it by another value to use it");
}

Value *Variable::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("variable object just hold variable name in memory, replace it by another value to use it");
}

Value *Variable::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("variable object just hold variable name in memory, replace it by another value to use it");
}

Value *Variable::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("variable object just hold variable name in memory, replace it by another value to use it");
}

Value *Variable::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("variable object just hold variable name in memory, replace it by another value to use it");
}

Value *Variable::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("variable object just hold variable name in memory, replace it by another value to use it");
}

Value *Variable::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("variable object just hold variable name in memory, replace it by another value to use it");
}

Value *Variable::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("variable object just hold variable name in memory, replace it by another value to use it");
}

Value *Variable::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("variable object just hold variable name in memory, replace it by another value to use it");
}

Value *Variable::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("variable object just hold variable name in memory, replace it by another value to use it");
}

Value *Variable::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("variable object just hold variable name in memory, replace it by another value to use it");
}
