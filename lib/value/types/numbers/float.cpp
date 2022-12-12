#include "value/types/numbers/float.hpp"

Float::Float(const std::string &value, TextRange range, ValueOwner owner) : 
	Value(FLOAT, range, owner),
	value(std::stof(value)){}

Float::Float(float value, TextRange range, ValueOwner owner) : 
	Value(FLOAT, range, owner),
	value(value) {}

std::unique_ptr<Float> Float::emptyFloat = std::make_unique<Float>(0.0f, TextRange(), Value::EMPTY_VALUE);

bool Float::isCastableTo(ValueType type) const {
	return 
		type == STRING ||
		type == INT ||
		type == FLOAT ||
		type == BOOL;
}

Value *Float::to(ValueType type, ValueOwner owner) const {
	switch (type) {
		case STRING:
			return new String(std::to_string(value), range, owner);
		case INT:
			return new Int(static_cast<int64_t>(value), range, owner);
		case FLOAT:
			return new Float(this->value, this->range, owner);
		case BOOL:
			return new Bool(value != 0, range, owner);
		default:
			throw std::runtime_error("Invalid value type");
	};
}

 Value *Float::copy(ValueOwner owner) const {
	return new Float(value, range, owner);
}

inline std::string Float::getStringValue() const {
	std::string value = std::to_string(this->value);
	// remove trailing zeros and decimal point if there are no digits after it
	value.erase(value.find_last_not_of('0') + 1, std::string::npos);
	if (value.back() == '.') value.pop_back();
	return  value;
}

operatorResult Float::opadd(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot add value of type " + other->getStringType() + " to Float",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_pair(
				ExpressionResult(),
				new Float(value + static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Float(value + static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case FLOAT:
			return std::make_pair(
				ExpressionResult(),
				new Float(value + static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		default:
			return std::make_pair(ExpressionResult(),	nullptr);
	};
}

operatorResult Float::opsub(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot subtract value of type " + other->getStringType() + " from Int",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_pair(
				ExpressionResult(),
				new Float(value - static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Float(value - static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case FLOAT:
			return std::make_pair(
				ExpressionResult(),
				new Float(value - static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		default:
			return std::make_pair(ExpressionResult(),	nullptr);
	};
}

operatorResult Float::opmul(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot multiply INT by value of type " + other->getStringType(),
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_pair(
				ExpressionResult(),
				new Float(value * static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Float(value * static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case FLOAT:
			return std::make_pair(
				ExpressionResult(),
				new Float(value * static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		default:
			return std::make_pair(ExpressionResult(),	nullptr);
	};
}

operatorResult Float::opdiv(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot divide INT number by value of type " + other->getStringType(),
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT: {
			int64_t otherValue = static_cast<Int const*>(other)->getValue();
			if (otherValue == 0)
				return std::make_pair(
					ExpressionResult(
						"Division by 0",
						other->getRange(),
						context
					),
					Int::empty()
				);
			return std::make_pair(
				ExpressionResult(),
				new Float(value / otherValue, range, Value::INTERPRETER)
			);
		}
		case BOOL: {
			bool otherValue = static_cast<Bool const*>(other)->getValue();
			if (!otherValue)
				return std::make_pair(
					ExpressionResult(
						"Division by false",
						other->getRange(),
						context
					),
					Int::empty()
				);
			return std::make_pair(
				ExpressionResult(),
				new Float(value / otherValue, range, Value::INTERPRETER)
			);
		}
		case FLOAT: {
			float otherValue = static_cast<Float const*>(other)->getValue();
			if (otherValue == 0.0)
				return std::make_pair(
					ExpressionResult(
						"Division by 0.0",
						other->getRange(),
						context
					),
					Float::empty()
				);
			return std::make_pair(
				ExpressionResult(),
				new Float(value / otherValue, range, Value::INTERPRETER)
			);
		}
		default:
			return std::make_pair(ExpressionResult(),	nullptr);
	};
}

operatorResult Float::opmod(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot modulo INT by value of type " + other->getStringType(),
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT: {
			int64_t otherValue = static_cast<Int const*>(other)->getValue();
			if (otherValue == 0)
				return std::make_pair(
					ExpressionResult(
						"Modulo by 0",
						other->getRange(),
						context
					),
					nullptr
				);
			return std::make_pair(
				ExpressionResult(),
				new Float(std::fmod(value, otherValue), range, Value::INTERPRETER)
			);
		}
		case BOOL: {
			bool otherValue = static_cast<Bool const*>(other)->getValue();
			if (!otherValue)
				return std::make_pair(
					ExpressionResult(
						"Modulo by false",
						other->getRange(),
						context
					),
					nullptr
				);
			return std::make_pair(
				ExpressionResult(),
				new Float(std::fmod(value, otherValue), range, Value::INTERPRETER)
			);
		}
		case FLOAT: {
			float otherValue = static_cast<Float const*>(other)->getValue();
			if (otherValue == 0.0)
				return std::make_pair(
					ExpressionResult(
						"Modulo by 0.0",
						other->getRange(),
						context
					),
					Float::empty()
				);
			return std::make_pair(
				ExpressionResult(),
				new Float(std::fmod(value, otherValue), range, Value::INTERPRETER)
			);
		}
		default:
			return std::make_pair(ExpressionResult(),	nullptr);
	};
}

operatorResult Float::oppow(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot raise value of type " + other->getStringType() + " to Float",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_pair(
				ExpressionResult(),
				new Float(std::pow(value, static_cast<Int const*>(other)->getValue()), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Float(std::pow(value, static_cast<Bool const*>(other)->getValue()), range, Value::INTERPRETER)
			);
		case FLOAT:
			return std::make_pair(
				ExpressionResult(),
				new Float(std::pow(value, static_cast<Float const*>(other)->getValue()), range, Value::INTERPRETER)
			);
		default:
			return std::make_pair(ExpressionResult(),	nullptr);
	};
}

operatorResult Float::opgt(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot compare value of type " + other->getStringType() + " to Float",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value > static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value > static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case FLOAT:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value > static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		default:
			return std::make_pair(ExpressionResult(),	nullptr);
	};
}

operatorResult Float::opge(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot compare value of type " + other->getStringType() + " to Float",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value >= static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value >= static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case FLOAT:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value >= static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		default:
			return std::make_pair(ExpressionResult(),	nullptr);
	};
}

operatorResult Float::oplt(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot compare value of type " + other->getStringType() + " to Float",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value < static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value < static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case FLOAT:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value < static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		default:
			return std::make_pair(ExpressionResult(),	nullptr);
	};
}

operatorResult Float::ople(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot compare value of type " + other->getStringType() + " to Float",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value <= static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value <= static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case FLOAT:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value <= static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		default:
			return std::make_pair(ExpressionResult(),	nullptr);
	};
}

operatorResult Float::opne(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(),
			new Bool(true, range, Value::INTERPRETER)
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value != static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value != static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case FLOAT:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value != static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		default:
			return std::make_pair(ExpressionResult(),	nullptr);
	};
}

operatorResult Float::opeq(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(),
			new Bool(false, range, Value::INTERPRETER)
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value == static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value == static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case FLOAT:
			return std::make_pair(
				ExpressionResult(),
				new Bool(value == static_cast<Float const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		default:
			return std::make_pair(ExpressionResult(),	nullptr);
	};
}