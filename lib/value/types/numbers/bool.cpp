#include "value/types/numbers/bool.hpp"

Bool::Bool(const std::string &value, TextRange range, ValueOwner owner, const TextRange variableRange) : 
	Value(BOOL, range, owner, variableRange),
	value(value != "false"){}

Bool::Bool(bool value, TextRange range, ValueOwner owner, const TextRange variableRange) : 
	Value(BOOL, range, owner, variableRange),
	value(value){}

std::unique_ptr<Bool> Bool::emptyBool = std::make_unique<Bool>(false, TextRange(), Value::EMPTY_VALUE);


Value *Bool::to(ValueType type, ValueOwner owner) const {
	switch (type) {
		case STRING:
			return new String(value ? "true" : "false", range, owner);
		case INT:
			return new Int(static_cast<int64_t>(value), range, owner);
		case FLOAT:
			return new Float(static_cast<float>(value), range, owner);
		case BOOL:
		case ANY:
			return new Bool(this->value, this->range, owner);
		default:
			throw std::runtime_error("Invalid value type");
	};
}

Value *Bool::copy(ValueOwner owner) const {
	return new Bool(value, range, owner, variableRange);
}

inline std::string Bool::getStringValue() const {
	return  value ? "true" : "false";
}

Value *Bool::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Int(value + static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Int(value + static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(value + static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot add value of type " + other->getStringType() + " to Bool");
	};
}

Value *Bool::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Int(value - static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Int(value - static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(value - static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot subtract value of type " + other->getStringType() + " from Bool");
	};
}

Value *Bool::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Int(value * static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Int(value * static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(value * static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot multiply value of type " + other->getStringType() + " by Bool");
	};
}

Value *Bool::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Int(value / static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Int(value / static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(value / static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot divide value of type " + other->getStringType() + " by Bool");
	};
}

Value *Bool::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT: 
			return new Int(value % static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Int(value % static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(std::fmod(value, static_cast<Float const*>(other)->getValue()), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot modulo value of type " + other->getStringType() + " by Bool");
	};
}

Value *Bool::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Int(std::pow(value, static_cast<Int const*>(other)->getValue()), range, Value::INTERPRETER);
		case BOOL:
			return new Int(std::pow(value, static_cast<Bool const*>(other)->getValue()), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(std::pow(value, static_cast<Float const*>(other)->getValue()), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot raise Bool to the power of value of type " + other->getStringType());
	};
}

Value *Bool::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value > static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value > static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value > static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " to Bool");
	};
}

Value *Bool::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value >= static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value >= static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value >= static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " to Bool");
	};
}

Value *Bool::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value < static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value < static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value < static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " to Bool");
	};
}

Value *Bool::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value <= static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value <= static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value <= static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " to Bool");
	};
}

Value *Bool::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value != static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value != static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value != static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			return new Bool(true, range, Value::INTERPRETER);
	};
}

Value *Bool::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value == static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value == static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value == static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			return new Bool(false, range, Value::INTERPRETER);
	};
}