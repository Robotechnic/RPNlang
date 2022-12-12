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
	operatorResult result = this->value->opadd(other, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator-(const Value* other) {
	operatorResult result = this->value->opsub(other, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator*(const Value* other) {
	operatorResult result = this->value->opmul(other, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator/(const Value* other) {
	operatorResult result = this->value->opdiv(other, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator%(const Value* other) {
	operatorResult result = this->value->opmod(other, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator^(const Value* other) {
	operatorResult result = this->value->oppow(other, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}


void CPPInterface::operator+(const CPPInterface& other) {
	operatorResult result = this->value->opadd(other.value, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator-(const CPPInterface& other) {
	operatorResult result = this->value->opsub(other.value, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator*(const CPPInterface& other) {
	operatorResult result = this->value->opmul(other.value, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator/(const CPPInterface& other) {
	operatorResult result = this->value->opdiv(other.value, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator%(const CPPInterface& other) {
	operatorResult result = this->value->opmod(other.value, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator^(const CPPInterface& other) {
	operatorResult result = this->value->oppow(other.value, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator++() {
	Int add = Int(1, TextRange(), Value::PARENT_FUNCTION);
	operatorResult result = this->value->opadd(&add, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result.second;
}

void CPPInterface::operator--() {
	Int sub = Int(1, TextRange(), Value::PARENT_FUNCTION);
	operatorResult result = this->value->opsub(&sub, nullptr);
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
	operatorResult result = left.getValue()->opeq(right, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	bool comp = static_cast<Bool *>(result.second)->getValue();
	delete result.second;
	return comp;
}

bool operator!=(const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->opne(right, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	bool comp = static_cast<Bool *>(result.second)->getValue();
	delete result.second;
	return comp;
}

bool operator< (const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->oplt(right, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	bool comp = static_cast<Bool *>(result.second)->getValue();
	delete result.second;
	return comp;
}

bool operator> (const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->opgt(right, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	bool comp = static_cast<Bool *>(result.second)->getValue();
	delete result.second;
	return comp;
}

bool operator<=(const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->ople(right, nullptr);
	if (result.first.error()) {
		throw std::runtime_error(result.first.getErrorMessage());
	}
	bool comp = static_cast<Bool *>(result.second)->getValue();
	delete result.second;
	return comp;
}

bool operator>=(const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->opge(right, nullptr);
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

