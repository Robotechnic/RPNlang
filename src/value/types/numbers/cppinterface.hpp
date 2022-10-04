#pragma once

#include "value/value.hpp"
#include "value/types/numbers/bool.hpp"
#include "expressionresult/expressionresult.hpp"

class CPPInterface {
	public:
		CPPInterface();
		CPPInterface(Value *value);
		CPPInterface(const CPPInterface &other);
		Value *getValue() const;

		void operator=(const CPPInterface &other);
		void operator=(Value *other);

		// cpp operators maped to value operators
		void operator+(const Value* other);
		void operator-(const Value* other);
		void operator*(const Value* other);
		void operator/(const Value* other);
		void operator%(const Value* other);
		void operator^(const Value* other);

		void operator+(const CPPInterface& other);
		void operator-(const CPPInterface& other);
		void operator*(const CPPInterface& other);
		void operator/(const CPPInterface& other);
		void operator%(const CPPInterface& other);
		void operator^(const CPPInterface& other);

		void operator++();
		void operator--();

	private:
		Value *value;
};

CPPInterface& operator+=(CPPInterface& left, const Value* right);
CPPInterface& operator-=(CPPInterface& left, const Value* right);
CPPInterface& operator*=(CPPInterface& left, const Value* right);
CPPInterface& operator/=(CPPInterface& left, const Value* right);
CPPInterface& operator%=(CPPInterface& left, const Value* right);
CPPInterface& operator^=(CPPInterface& left, const Value* right);

CPPInterface& operator+=(CPPInterface& left, const CPPInterface& right);
CPPInterface& operator-=(CPPInterface& left, const CPPInterface& right);
CPPInterface& operator*=(CPPInterface& left, const CPPInterface& right);
CPPInterface& operator/=(CPPInterface& left, const CPPInterface& right);
CPPInterface& operator%=(CPPInterface& left, const CPPInterface& right);
CPPInterface& operator^=(CPPInterface& left, const CPPInterface& right);

bool operator==(const CPPInterface& left, const Value* right);
bool operator!=(const CPPInterface& left, const Value* right);
bool operator< (const CPPInterface& left, const Value* right);
bool operator> (const CPPInterface& left, const Value* right);
bool operator<=(const CPPInterface& left, const Value* right);
bool operator>=(const CPPInterface& left, const Value* right);

bool operator==(const CPPInterface& left, const CPPInterface& right);
bool operator!=(const CPPInterface& left, const CPPInterface& right);
bool operator< (const CPPInterface& left, const CPPInterface& right);
bool operator> (const CPPInterface& left, const CPPInterface& right);
bool operator<=(const CPPInterface& left, const CPPInterface& right);
bool operator>=(const CPPInterface& left, const CPPInterface& right);
