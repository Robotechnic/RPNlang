#pragma once

#include <stdexcept>
#include <string>
#include <variant>

enum ValueType {
	INT,
	FLOAT,
	STRING,
	VARIABLE,
	BUILTIN_VARIABLE,
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

using RPNBaseType = std::variant<std::string, ValueType>;

class RPNValueType {
  public:
	RPNValueType() = default;
	RPNValueType(const RPNBaseType &type, const RPNBaseType &listType);
	RPNValueType(const RPNBaseType &type);
	RPNValueType(const ValueType &type);
	RPNValueType(const std::string &type);
	RPNValueType(const char *type);
	explicit RPNValueType(RPNBaseType &&type);
	explicit RPNValueType(std::string &&type);

	static bool isCastableTo(ValueType from, ValueType to);

	static ValueType lessRestrictive(ValueType type1, ValueType type2);

	int index() const;

	std::string name() const;

	static std::string typeName(const RPNBaseType &type);

	bool operator==(const RPNValueType &other);

	bool operator==(RPNBaseType other);

	bool operator!=(const RPNValueType &other);

	bool operator!=(RPNBaseType other);

	RPNBaseType getType() const;

	RPNBaseType getListType() const;
	void setListType(const RPNBaseType &type);

  private:
	RPNBaseType type;
	RPNBaseType listType;
};
