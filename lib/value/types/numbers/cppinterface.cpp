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
	Value *result = this->value->opadd(other, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}

void CPPInterface::operator-(const Value* other) {
	Value *result = this->value->opsub(other, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}

void CPPInterface::operator*(const Value* other) {
	Value *result = this->value->opmul(other, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}

void CPPInterface::operator/(const Value* other) {
	Value *result = this->value->opdiv(other, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}

void CPPInterface::operator%(const Value* other) {
	Value *result = this->value->opmod(other, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}

void CPPInterface::operator^(const Value* other) {
	Value *result = this->value->oppow(other, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}


void CPPInterface::operator+(const CPPInterface& other) {
	Value *result = this->value->opadd(other.value, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}

void CPPInterface::operator-(const CPPInterface& other) {
	Value *result = this->value->opsub(other.value, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}

void CPPInterface::operator*(const CPPInterface& other) {
	Value *result = this->value->opmul(other.value, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}

void CPPInterface::operator/(const CPPInterface& other) {
	Value *result = this->value->opdiv(other.value, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}

void CPPInterface::operator%(const CPPInterface& other) {
	Value *result = this->value->opmod(other.value, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}

void CPPInterface::operator^(const CPPInterface& other) {
	Value *result = this->value->oppow(other.value, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}

void CPPInterface::operator++() {
	Int add = Int(1, TextRange(), Value::PARENT_FUNCTION);
	Value *result = this->value->opadd(&add, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
}

void CPPInterface::operator--() {
	Int sub = Int(1, TextRange(), Value::PARENT_FUNCTION);
	Value *result = this->value->opsub(&sub, TextRange(), nullptr);
	Value::deleteValue(&this->value, Value::INTERPRETER);
	this->value = result;
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
	Value *result = left.getValue()->opeq(right, TextRange(), nullptr);
	bool comp = static_cast<Bool *>(result)->getValue();
	delete result;
	return comp;
}

bool operator!=(const CPPInterface& left, const Value* right) {
	Value *result = left.getValue()->opne(right, TextRange(), nullptr);
	bool comp = static_cast<Bool *>(result)->getValue();
	delete result;
	return comp;
}

bool operator< (const CPPInterface& left, const Value* right) {
	Value *result = left.getValue()->oplt(right, TextRange(), nullptr);
	bool comp = static_cast<Bool *>(result)->getValue();
	delete result;
	return comp;
}

bool operator> (const CPPInterface& left, const Value* right) {
	Value *result = left.getValue()->opgt(right, TextRange(), nullptr);
	bool comp = static_cast<Bool *>(result)->getValue();
	delete result;
	return comp;
}

bool operator<=(const CPPInterface& left, const Value* right) {
	Value *result = left.getValue()->ople(right, TextRange(), nullptr);
	bool comp = static_cast<Bool *>(result)->getValue();
	delete result;
	return comp;
}

bool operator>=(const CPPInterface& left, const Value* right) {
	Value *result = left.getValue()->opge(right, TextRange(), nullptr);
	bool comp = static_cast<Bool *>(result)->getValue();
	delete result;
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

