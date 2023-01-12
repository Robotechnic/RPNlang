#include "value/types/numbers/cppinterface.hpp"

CPPInterface::CPPInterface(Value *value) : value(value) {}
CPPInterface::CPPInterface(const CPPInterface &other) : value(other.value) {}

Value *CPPInterface::getValue() const {
	return value;
}

Value *&CPPInterface::getValue() {
	return value;
}

void CPPInterface::operator=(const CPPInterface &other) {
	this->value = other.value;
}

void CPPInterface::operator=(Value *other) {
	this->value = other;
}

void CPPInterface::operator+(const Value* other) {
	operatorResult result = this->value->opadd(other, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator-(const Value* other) {
	operatorResult result = this->value->opsub(other, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator*(const Value* other) {
	operatorResult result = this->value->opmul(other, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator/(const Value* other) {
	operatorResult result = this->value->opdiv(other, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator%(const Value* other) {
	operatorResult result = this->value->opmod(other, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator^(const Value* other) {
	operatorResult result = this->value->oppow(other, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}


void CPPInterface::operator+(const CPPInterface& other) {
	operatorResult result = this->value->opadd(other.value, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator-(const CPPInterface& other) {
	operatorResult result = this->value->opsub(other.value, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator*(const CPPInterface& other) {
	operatorResult result = this->value->opmul(other.value, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator/(const CPPInterface& other) {
	operatorResult result = this->value->opdiv(other.value, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator%(const CPPInterface& other) {
	operatorResult result = this->value->opmod(other.value, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator^(const CPPInterface& other) {
	operatorResult result = this->value->oppow(other.value, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator++() {
	Int add = Int(1, TextRange(), Value::PARENT_FUNCTION);
	operatorResult result = this->value->opadd(&add, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator--() {
	Int sub = Int(1, TextRange(), Value::PARENT_FUNCTION);
	operatorResult result = this->value->opsub(&sub, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}


CPPInterface& operator+=(CPPInterface& left, const Value* right) {
	left.operator+(right);
	return left;
}

CPPInterface& operator-=(CPPInterface& left, const Value* right) {
	left.operator-(right);
	return left;
}

CPPInterface& operator*=(CPPInterface& left, const Value* right) {
	left.operator*(right);
	return left;
}

CPPInterface& operator/=(CPPInterface& left, const Value* right) {
	left.operator/(right);
	return left;
}

CPPInterface& operator%=(CPPInterface& left, const Value* right) {
	left.operator%(right);
	return left;
}

CPPInterface& operator^=(CPPInterface& left, const Value* right) {
	left.operator^(right);
	return left;
}


CPPInterface& operator+=(CPPInterface& left, const CPPInterface& right) {
	left.operator+(right);
	return left;
}

CPPInterface& operator-=(CPPInterface& left, const CPPInterface& right) {
	left.operator-(right);
	return left;
}

CPPInterface& operator*=(CPPInterface& left, const CPPInterface& right) {
	left.operator*(right);
	return left;
}

CPPInterface& operator/=(CPPInterface& left, const CPPInterface& right) {
	left.operator/(right);
	return left;
}

CPPInterface& operator%=(CPPInterface& left, const CPPInterface& right) {
	left.operator%(right);
	return left;
}

CPPInterface& operator^=(CPPInterface& left, const CPPInterface& right) {
	left.operator^(right);
	return left;
}

bool operator==(const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->opeq(right, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	bool comp = static_cast<Bool *>(result.second)->getValue();
	delete result.second;
	return comp;
}

bool operator!=(const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->opne(right, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	bool comp = static_cast<Bool *>(result.second)->getValue();
	delete result.second;
	return comp;
}

bool operator< (const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->oplt(right, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	bool comp = static_cast<Bool *>(result.second)->getValue();
	delete result.second;
	return comp;
}

bool operator> (const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->opgt(right, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	bool comp = static_cast<Bool *>(result.second)->getValue();
	delete result.second;
	return comp;
}

bool operator<=(const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->ople(right, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	bool comp = static_cast<Bool *>(result.second)->getValue();
	delete result.second;
	return comp;
}

bool operator>=(const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->opge(right, TextRange(), nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	bool comp = static_cast<Bool *>(result.second)->getValue();
	delete result.second;
	return comp;
}

bool operator==(const CPPInterface& left, const CPPInterface& right) {
	return left == right.getValue();
}

bool operator!=(const CPPInterface& left, const CPPInterface& right) {
	return left != right.getValue();
}

bool operator< (const CPPInterface& left, const CPPInterface& right) {
	return left < right.getValue();
}

bool operator> (const CPPInterface& left, const CPPInterface& right) {
	return left > right.getValue();
}

bool operator<=(const CPPInterface& left, const CPPInterface& right) {
	return left <= right.getValue();
}

bool operator>=(const CPPInterface& left, const CPPInterface& right) {
	return left >= right.getValue();
}

