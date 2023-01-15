#pragma once


#include <unordered_map>
#include <string>
#include <vector>
#include <string_view>
#include <ostream>
#include <any>

#include "value/value.hpp"
#include "value/types/numbers/bool.hpp"
#include "value/types/path.hpp"

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
		std::unordered_map<std::string, ValueType> members;
		std::vector<std::string> memberOrder;

		friend class Struct;
};

std::ostream &operator<<(std::ostream &stream, const StructDefinition &definition);

class Struct : public Value {
	public:
		Struct(TextRange range, std::string_view name, ValueOwner owner, bool immutable = false);
		~Struct() override;
		ExpressionResult setMembers(std::vector<Value*> members, ContextPtr context);

		// this is for struct which are used by the interpreter
		ExpressionResult setMember(const Path *member, Value *value, ContextPtr context, Value **hold);
		ExpressionResult getMember(const Path *member, Value *&value, ContextPtr context);

		// this is for struct which are used by the c++ code
		void setMember(std::string_view member, Value *value, ContextPtr context);
		Value* getMember(std::string_view member, ContextPtr context);

		bool isCastableTo(ValueType type) const;
		bool isNumber() const;

		Value *to(ValueType type, ValueOwner owner = INTERPRETER) const;
		inline Value *copy(ValueOwner owner = INTERPRETER) const;
		
		inline std::string getStringValue() const;
		std::string_view getStructName() const;

		// this is only for struct which are used directly by c++ code
		void setData(std::shared_ptr<void> data);
		std::shared_ptr<void> getData();

		operatorResult opadd(const Value *other, const TextRange &range, const ContextPtr &context) const;
		operatorResult opsub(const Value *other, const TextRange &range, const ContextPtr &context) const;
		operatorResult opmul(const Value *other, const TextRange &range, const ContextPtr &context) const;
		operatorResult opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const;
		operatorResult opmod(const Value *other, const TextRange &range, const ContextPtr &context) const;
		operatorResult oppow(const Value *other, const TextRange &range, const ContextPtr &context) const;
		
		operatorResult opgt(const Value *other, const TextRange &range, const ContextPtr &context) const;
		operatorResult opge(const Value *other, const TextRange &range, const ContextPtr &context) const;
		operatorResult oplt(const Value *other, const TextRange &range, const ContextPtr &context) const;
		operatorResult ople(const Value *other, const TextRange &range, const ContextPtr &context) const;
		operatorResult opne(const Value *other, const TextRange &range, const ContextPtr &context) const;
		operatorResult opeq(const Value *other, const TextRange &range, const ContextPtr &context) const;

		static void addStructDefinition(StructDefinition &definition);
		static int  getStructMembersCount(std::string_view structName);
		static bool structExists(std::string_view structName);
		static ExpressionResult getStruct(const Path *path, Value *&structValue, ContextPtr context);

		bool immutable;

	private:
		StructDefinition *definition;
		std::shared_ptr<std::unordered_map<std::string, Value*>> members;
		std::shared_ptr<void> data;

		static std::unordered_map<std::string, StructDefinition> definitions;
};