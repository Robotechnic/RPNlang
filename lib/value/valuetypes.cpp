#include "value/valuetypes.hpp"

/**
 * @brief return the string representation of the ValueType
 * 
 * @param type the type to convert to a string
 * @return std::string the string representation of the type
 */
std::string stringType(const ValueType &type) {
	switch(type) {
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