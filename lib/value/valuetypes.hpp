#pragma once

#include <variant>
#include <string>
#include <stdexcept>

enum ValueType {
	INT,
	FLOAT,
	STRING,
	VARIABLE,
	FUNCTION,
	BOOL,
	PATH,
	BUILTIN_PATH,
	STRUCT_ACCESS,
	LIST,
	LIST_ELEMENT,
	STRUCT,
	ANY,
	NONE
};

std::string stringType(const ValueType &type);
ValueType stringToType(std::string_view type);

typedef std::variant<std::string, ValueType> RPNBaseType;

struct RPNValueType {
	RPNValueType() = default;
	RPNValueType(const RPNBaseType &type, const RPNBaseType &listType) : type(type), listType(listType) {};
	RPNValueType(const RPNBaseType &type) : type(type), listType(NONE) {};
	RPNValueType(const ValueType &type) : type(type), listType(NONE) {};
	RPNValueType(const std::string &type) : type(type), listType(NONE) {};
	RPNValueType(const char *type) : type(std::string(type)), listType(NONE) {};
	explicit RPNValueType(RPNBaseType &&type) : type(type), listType(NONE) {};
	explicit RPNValueType(std::string &&type) : type(type), listType(NONE) {};

	static bool isCastableTo(ValueType from, ValueType to) {
		if (from == to || to == ANY || to == BOOL) {
			return true;
		}
		switch (from) {
			case INT:
			case FLOAT:
			case BOOL:
				return to == INT || to == FLOAT || to == BOOL || to == STRING;
			case STRING:
				return to == STRING || to == BOOL;
			case LIST:
				return to == LIST || to == STRING || to == BOOL;
			default:
				return false;
		}
	}

	static ValueType lessRestrictive(ValueType type1, ValueType type2) {
		return type1 >= type2 ? type1 : type2;
	}

	int index() const {
		return type.index();
	};
	std::string name() const {
		if (type.index() == 0) {
			return std::get<0>(type);
		} else if (std::get<1>(type) == ValueType::LIST) {
			return stringType(std::get<1>(type)) + "[" + (
				listType.index() == 0 ? std::get<std::string>(listType) : 
										stringType(std::get<ValueType>(listType))
			) + "]";
		} else {
			return stringType(std::get<1>(type));
		}
	};
	static std::string typeName(const RPNBaseType &type) {
		if (type.index() == 0) {
			return std::get<0>(type);
		} else if (std::get<1>(type) == ValueType::LIST) {
			return stringType(std::get<1>(type)) + "[]";
		} else {
			return stringType(std::get<1>(type));
		}
	};

	bool operator==(const RPNValueType &other) {
		return this->type.index() == other.type.index() && (
			this->type.index() == 0 ? std::get<0>(this->type) == std::get<0>(other.type) :
									  RPNValueType::isCastableTo(std::get<1>(this->type), std::get<1>(other.type))
		) && (
			(other.listType.index() == 1 && std::get<1>(other.listType) == ANY) || (
			this->listType.index() == other.listType.index() && (
				this->listType.index() == 0 ? std::get<0>(this->listType) == std::get<0>(other.listType) :
											  RPNValueType::isCastableTo(std::get<1>(this->listType), std::get<1>(other.listType))
			))
		);
	};

	bool operator==(const RPNBaseType &other) {
		return this->type.index() == other.index() && (
			this->type.index() == 0 ? std::get<0>(this->type) == std::get<0>(other) :
									  RPNValueType::isCastableTo(std::get<1>(this->type), std::get<1>(other))
		);
	};

	bool operator!=(const RPNValueType &other) {
		return !(*this == other);
	};

	bool operator!=(RPNBaseType other) {
		return !(*this == other);
	};

	RPNBaseType type;
	RPNBaseType listType;
};
