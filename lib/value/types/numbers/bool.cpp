#include "value/types/numbers/bool.hpp"

Bool::Bool(const std::string &value, TextRange range, ValueOwner owner) : 
	Value(BOOL, range, owner),
	value(value != "false"){}

Bool::Bool(bool value, TextRange range, ValueOwner owner) : 
	Value(BOOL, range, owner),
	value(value){}

std::unique_ptr<Bool> Bool::emptyBool = std::make_unique<Bool>(false, TextRange(), Value::EMPTY_VALUE);

bool Bool::isCastableTo(ValueType type) const {
	return 
		type == STRING ||
		type == ANY ||
		type == INT ||
		type == FLOAT ||
		type == BOOL;
}

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
	return new Bool(value, range, owner);
}

inline std::string Bool::getStringValue() const {
	return  value ? "true" : "false";
}

operatorResult Bool::opadd(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot add value of type " + other->getStringType() + " to Bool",
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

operatorResult Bool::opsub(const Value *other, const ContextPtr &context) const {
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

operatorResult Bool::opmul(const Value *other, const ContextPtr &context) const {
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

operatorResult Bool::opdiv(const Value *other, const ContextPtr &context) const {
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

operatorResult Bool::opmod(const Value *other, const ContextPtr &context) const {
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

operatorResult Bool::oppow(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot raise value of type " + other->getStringType() + " to Bool",
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

operatorResult Bool::opgt(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot compare value of type " + other->getStringType() + " to Bool",
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

operatorResult Bool::opge(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot compare value of type " + other->getStringType() + " to Bool",
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

operatorResult Bool::oplt(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot compare value of type " + other->getStringType() + " to Bool",
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

operatorResult Bool::ople(const Value *other, const ContextPtr &context) const {
	if (!other->isNumber())
		return std::make_pair(
			ExpressionResult(
				"Cannot compare value of type " + other->getStringType() + " to Bool",
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

operatorResult Bool::opne(const Value *other, const ContextPtr &context) const {
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

operatorResult Bool::opeq(const Value *other, const ContextPtr &context) const {
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