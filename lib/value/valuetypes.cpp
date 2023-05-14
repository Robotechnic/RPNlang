#include "value/valuetypes.hpp"

/**
 * @brief return the string representation of the ValueType
 *
 * @param type the type to convert to a string
 * @return std::string the string representation of the type
 */
std::string stringType(const ValueType &type) {
	switch (type) {
		case INT:
			return "int";
		case FLOAT:
			return "float";
		case STRING:
			return "string";
		case BOOL:
			return "bool";
		case FUNCTION:
			return "function";
		case NONE:
			return "none";
		case VARIABLE:
			return "variable";
		case PATH:
			return "path";
		case LIST:
			return "list";
		case ANY:
			return "any";
		case STRUCT:
			return "struct";
		default:
			throw std::runtime_error("This type doesn't exist " + std::to_string(type));
	}
}

/**
 * @brief convert string representation of a value type to a ValueType
 *
 * @param type the string representation of the type
 * @return ValueType the type
 */
ValueType stringToType(std::string_view type) {
	if (type == "int")
		return INT;
	if (type == "float")
		return FLOAT;
	if (type == "string")
		return STRING;
	if (type == "function")
		return FUNCTION;
	if (type == "bool")
		return BOOL;
	if (type == "none")
		return NONE;
	if (type == "list")
		return LIST;

	throw std::runtime_error("This string type doesn't exist");
}

RPNValueType::RPNValueType(const RPNBaseType &type, const RPNBaseType &listType)
	: type(type), listType(listType){};
RPNValueType::RPNValueType(const RPNBaseType &type) : type(type), listType(NONE){};
RPNValueType::RPNValueType(const ValueType &type) : type(type), listType(NONE){};
RPNValueType::RPNValueType(const std::string &type) : type(type), listType(NONE){};
RPNValueType::RPNValueType(const char *type) : type(std::string(type)), listType(NONE){};
RPNValueType::RPNValueType(RPNBaseType &&type) : type(std::move(type)), listType(NONE){};
RPNValueType::RPNValueType(std::string &&type) : type(std::move(type)), listType(NONE){};

bool RPNValueType::isCastableTo(ValueType from, ValueType to) {
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

ValueType RPNValueType::lessRestrictive(ValueType type1, ValueType type2) {
	return type1 >= type2 ? type1 : type2;
}

int RPNValueType::index() const {
	return type.index();
};

std::string RPNValueType::name() const {
	if (type.index() == 0) {
		return std::get<0>(type);
	} else if (std::get<1>(type) == ValueType::LIST) {
		return stringType(std::get<1>(type)) + "[" +
			   (listType.index() == 0 ? std::get<std::string>(listType)
									  : stringType(std::get<ValueType>(listType))) +
			   "]";
	} else {
		return stringType(std::get<1>(type));
	}
};

std::string RPNValueType::typeName(const RPNBaseType &type) {
	if (type.index() == 0) {
		return std::get<0>(type);
	} else if (std::get<1>(type) == ValueType::LIST) {
		return stringType(std::get<1>(type)) + "[]";
	} else {
		return stringType(std::get<1>(type));
	}
};

bool RPNValueType::operator==(const RPNValueType &other) {
	return this->type.index() == other.type.index() &&
		   (this->type.index() == 0
				? std::get<0>(this->type) == std::get<0>(other.type)
				: RPNValueType::isCastableTo(std::get<1>(this->type), std::get<1>(other.type))) &&
		   ((other.listType.index() == 1 && std::get<1>(other.listType) == ANY) ||
			(this->listType.index() == other.listType.index() &&
			 (this->listType.index() == 0
				  ? std::get<0>(this->listType) == std::get<0>(other.listType)
				  : RPNValueType::isCastableTo(std::get<1>(this->listType),
											   std::get<1>(other.listType)))));
};

bool RPNValueType::operator==(const RPNBaseType &other) {
	return this->type.index() == other.index() &&
		   (this->type.index() == 0
				? std::get<0>(this->type) == std::get<0>(other)
				: RPNValueType::isCastableTo(std::get<1>(this->type), std::get<1>(other)));
};

bool RPNValueType::operator!=(const RPNValueType &other) {
	return !(*this == other);
};

bool RPNValueType::operator!=(RPNBaseType other) {
	return !(*this == other);
};

RPNBaseType RPNValueType::getType() const {
	return type;
};

RPNBaseType RPNValueType::getListType() const {
	return listType;
};

void RPNValueType::setListType(const RPNBaseType &type) {
	this->listType = type;
};
