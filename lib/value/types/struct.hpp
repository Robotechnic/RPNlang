#pragma once


#include <map>
#include <string>
#include <vector>
#include <string_view>
#include <ostream>
#include <any>

#include "value/value.hpp"

class StructDefinition final {
	public:
		StructDefinition() = default;
		StructDefinition(std::string_view name);
		StructDefinition(const StructDefinition &other);
		StructDefinition(StructDefinition &&other);

		void operator=(StructDefinition &other);

		void addMember(std::string_view name, ValueType type);
		bool hasMember(std::string_view name, ValueType *memberType = nullptr) const;

		std::string getName() const;
		void display(std::ostream &stream) const;

	private:
		std::string name;
		std::map<std::string, ValueType> members;

		friend class Struct;
};

std::ostream &operator<<(std::ostream &stream, const StructDefinition &definition);

class Struct : public Value {
	public:
		Struct(TextRange range, std::string_view name, ValueOwner owner);
		ExpressionResult setMembers(std::vector<Value*> members, ContextPtr context);

		ExpressionResult setMember(Value *name, Value *value, ContextPtr context);
		ExpressionResult getMember(Value *name, Value *value, ContextPtr context);

		// this is only for struct which are used directly by c++ code
		void setData(std::any data);
		std::any getData();

		static void addStructDefinition(StructDefinition &definition);

	private:
		StructDefinition *definition;
		std::map<std::string, Value*> members;
		std::any data;


		static std::unordered_map<std::string, StructDefinition> definitions;
};