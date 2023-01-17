#include "value/types/numbers/int.hpp"

Int::Int(const std::string &value, TextRange range, ValueOwner owner, const TextRange variableRange) : 
	Value(INT, range, owner, variableRange),
	value(std::stoll(value)) {}

Int::Int(int64_t value, TextRange range, ValueOwner owner, const TextRange variableRange) : 
	Value(INT, range, owner, variableRange),
	value(value) {}

std::unique_ptr<Int> Int::emptyInt = std::make_unique<Int>(0, TextRange(), Value::EMPTY_VALUE);

Value *Int::to(ValueType type, ValueOwner owner) const {
	switch (type) {
		case STRING:
			return new String(std::to_string(value), range, owner);
		case INT:
		case ANY:
			return new Int(value, range, owner);
		case FLOAT:
			return new Float(static_cast<float>(value), range, owner);
		case BOOL:
			return new Bool(value != 0, range, owner);
		default:
			throw std::runtime_error("Invalid value type");
	};
}

 Value *Int::copy(ValueOwner owner) const {
	return new Int(value, range, owner, variableRange);
}

inline std::string Int::getStringValue() const {
	return std::to_string(value);
}

Value *Int::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Int(value + static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Int(value + static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(value + static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot add value of type " + other->getStringType() + " to an integer");
	};
}

Value *Int::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Int(value - static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Int(value - static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(value - static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot subtract value of type " + other->getStringType() + " to an integer");
	};
}

Value *Int::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Int(value * static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Int(value * static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(value * static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot multiply value of type " + other->getStringType() + " to an integer");
	};
}

Value *Int::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Int(value / static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Int(value / static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(value / static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot divide value of type " + other->getStringType() + " to an integer");
	};
}

Value *Int::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Int(value % static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Int(value % static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(std::fmod(value, static_cast<Float const*>(other)->getValue()), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot mod value of type " + other->getStringType() + " to an integer");
	};
}

Value *Int::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Int(std::pow(value, static_cast<Int const*>(other)->getValue()), range, Value::INTERPRETER);
		case BOOL:
			return new Int(std::pow(value, static_cast<Bool const*>(other)->getValue()), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(std::pow(value, static_cast<Float const*>(other)->getValue()), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot pow value of type " + other->getStringType() + " to an integer");
	};
}

Value *Int::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {	
	switch (other->getType()) {
		case INT:
			return new Bool(value > static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value > static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value > static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " to Int");
	};
}

Value *Int::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value >= static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value >= static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value >= static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " to Int");
	};
}

Value *Int::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value < static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value < static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value < static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " to Int");
	};
}

Value *Int::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value <= static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value <= static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value <= static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " to Int");
	};
}

Value *Int::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
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

Value *Int::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {	
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
