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

operatorResult Int::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot add value of type " + other->getStringType() + " to Int",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_pair(
				ExpressionResult(),
				new Int(value + static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Int(value + static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER)
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

operatorResult Int::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
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
				new Int(value - static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Int(value - static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER)
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

operatorResult Int::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {
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
				new Int(value * static_cast<Int const*>(other)->getValue(), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Int(value * static_cast<Bool const*>(other)->getValue(), range, Value::INTERPRETER)
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

operatorResult Int::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
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
				new Int(value / otherValue, range, Value::INTERPRETER)
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
				new Int(value / otherValue, range, Value::INTERPRETER)
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

operatorResult Int::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
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
				new Int(value % otherValue, range, Value::INTERPRETER)
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
				new Int(value % otherValue, range, Value::INTERPRETER)
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

operatorResult Int::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot raise value of type " + other->getStringType() + " to Int",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_pair(
				ExpressionResult(),
				new Int(std::pow(value, static_cast<Int const*>(other)->getValue()), range, Value::INTERPRETER)
			);
		case BOOL:
			return std::make_pair(
				ExpressionResult(),
				new Int(std::pow(value, static_cast<Bool const*>(other)->getValue()), range, Value::INTERPRETER)
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

operatorResult Int::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot compare value of type " + other->getStringType() + " to Int",
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

operatorResult Int::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot compare value of type " + other->getStringType() + " to Int",
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

operatorResult Int::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot compare value of type " + other->getStringType() + " to Int",
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

operatorResult Int::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot compare value of type " + other->getStringType() + " to Int",
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

operatorResult Int::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
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

operatorResult Int::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
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
