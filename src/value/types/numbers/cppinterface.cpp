#include "value/types/numbers/cppinterface.hpp"

CPPInterface::CPPInterface() {
	value = nullptr;
}
CPPInterface::CPPInterface(Value *value) : value(value) {}
CPPInterface::CPPInterface(const CPPInterface &other) : value(other.value) {}

Value *CPPInterface::getValue() const {
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
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}

void CPPInterface::operator-(const Value* other) {
	operatorResult result = this->value->opsub(other, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}

void CPPInterface::operator*(const Value* other) {
	operatorResult result = this->value->opmul(other, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}

void CPPInterface::operator/(const Value* other) {
	operatorResult result = this->value->opdiv(other, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}

void CPPInterface::operator%(const Value* other) {
	operatorResult result = this->value->opmod(other, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}

void CPPInterface::operator^(const Value* other) {
	operatorResult result = this->value->oppow(other, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}


void CPPInterface::operator+(const CPPInterface& other) {
	operatorResult result = this->value->opadd(other.value, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}

void CPPInterface::operator-(const CPPInterface& other) {
	operatorResult result = this->value->opsub(other.value, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}

void CPPInterface::operator*(const CPPInterface& other) {
	operatorResult result = this->value->opmul(other.value, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}

void CPPInterface::operator/(const CPPInterface& other) {
	operatorResult result = this->value->opdiv(other.value, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}

void CPPInterface::operator%(const CPPInterface& other) {
	operatorResult result = this->value->opmod(other.value, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}

void CPPInterface::operator^(const CPPInterface& other) {
	operatorResult result = this->value->oppow(other.value, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}

void CPPInterface::operator++() {
	Int add = Int(1, TextRange(), true);
	operatorResult result = this->value->opadd(&add, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
}

void CPPInterface::operator--() {
	Int sub = Int(1, TextRange(), true);
	operatorResult result = this->value->opsub(&sub, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	delete this->value;
	this->value = std::get<1>(result);
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
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	return static_cast<Bool *>(std::get<1>(result))->getValue();
}

bool operator!=(const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->opne(right, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	return static_cast<Bool *>(std::get<1>(result))->getValue();
}

bool operator< (const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->oplt(right, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	return static_cast<Bool *>(std::get<1>(result))->getValue();
}

bool operator> (const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->opgt(right, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	return static_cast<Bool *>(std::get<1>(result))->getValue();
}

bool operator<=(const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->ople(right, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	return static_cast<Bool *>(std::get<1>(result))->getValue();
}

bool operator>=(const CPPInterface& left, const Value* right) {
	operatorResult result = left.getValue()->opge(right, nullptr);
	if (std::get<0>(result).error()) {
		throw std::runtime_error(std::get<0>(result).getErrorMessage());
	}
	return static_cast<Bool *>(std::get<1>(result))->getValue();
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

