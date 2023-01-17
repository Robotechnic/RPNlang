#include "value/types/numbers/float.hpp"

Float::Float(const std::string &value, TextRange range, ValueOwner owner, const TextRange variableRange) : 
	Value(FLOAT, range, owner, variableRange),
	value(std::stof(value)){}

Float::Float(float value, TextRange range, ValueOwner owner, const TextRange variableRange) : 
	Value(FLOAT, range, owner, variableRange),
	value(value) {}

std::unique_ptr<Float> Float::emptyFloat = std::make_unique<Float>(0.0f, TextRange(), Value::EMPTY_VALUE);


Value *Float::to(ValueType type, ValueOwner owner) const {
	switch (type) {
		case STRING:
			return new String(std::to_string(value), range, owner);
		case INT:
			return new Int(static_cast<int64_t>(value), range, owner);
		case FLOAT:
		case ANY:
			return new Float(this->value, this->range, owner);
		case BOOL:
			return new Bool(value != 0, range, owner);
		default:
			throw std::runtime_error("Invalid value type");
	};
}

 Value *Float::copy(ValueOwner owner) const {
	return new Float(value, range, owner, variableRange);
}

inline std::string Float::getStringValue() const {
	std::string value = std::to_string(this->value);
	// remove trailing zeros and decimal point if there are no digits after it
	value.erase(value.find_last_not_of('0') + 1, std::string::npos);
	if (value.back() == '.') value.pop_back();
	return  value;
}

Value *Float::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Float(value + static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Float(value + static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(value + static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot add value of type " + other->getStringType() + " to float");
	};
}

Value *Float::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Float(value - static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Float(value - static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(value - static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot subtract value of type " + other->getStringType() + " from float");
	};
}

Value *Float::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {	
	switch (other->getType()) {
		case INT:
			return new Float(value * static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Float(value * static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(value * static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot multiply value of type float by " + other->getStringType());
	};
}

Value *Float::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT: 
			return new Float(value / static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Float(value / static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(value / static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot divide value of type float by " + other->getStringType());
	};
}

Value *Float::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Float(std::fmod(value, static_cast<Int const*>(other)->getValue()), range, Value::INTERPRETER);
		case BOOL:
			return new Float(std::fmod(value, static_cast<Bool const*>(other)->getValue()), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(std::fmod(value, static_cast<Float const*>(other)->getValue()), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot modulo value of type float to " + other->getStringType());
	};
}

Value *Float::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Float(std::pow(value, static_cast<Int const*>(other)->getValue()), range, Value::INTERPRETER);
		case BOOL:
			return new Float(std::pow(value, static_cast<Bool const*>(other)->getValue()), range, Value::INTERPRETER);
		case FLOAT:
			return new Float(std::pow(value, static_cast<Float const*>(other)->getValue()), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot raise value of type " + other->getStringType() + " to Float");
	};
}

Value *Float::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value > static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value > static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value > static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " to Float");
	};
}

Value *Float::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value >= static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value >= static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value >= static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " to Float");
	};
}

Value *Float::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value < static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value < static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value < static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " to Float");
	};
}

Value *Float::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	switch (other->getType()) {
		case INT:
			return new Bool(value <= static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER);
		case BOOL:
			return new Bool(value <= static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER);
		case FLOAT:
			return new Bool(value <= static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER);
		default:
			throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " to Float");
	};
}

Value *Float::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
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

Value *Float::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
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