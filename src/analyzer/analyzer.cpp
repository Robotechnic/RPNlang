#include "analyzer/analyzer.hpp"

Analyzer::Analyzer(ContextPtr context) : context(context) {}

void Analyzer::analyze(CodeBlock *codeblock) {
	for (auto it = codeblock->begin(); it != codeblock->end() && !this->hasErrors(); it++) {
		if ((*it)->getType() == LINE_BLOCK) {
			this->analyze(static_cast<Line *>(*it));
		} else if ((*it)->getType() == CODE_BLOCK) {
			this->analyze(static_cast<CodeBlock *>(*it));
		} else if ((*it)->getType() == FUNCTION_BLOCK) {
			this->analyze(static_cast<FunctionBlock *>(*it));
		} else {
			throw std::runtime_error("This block type is not supported");
		}
	}
}

bool Analyzer::hasErrors() const {
	return this->error.error();
}

ExpressionResult Analyzer::analyzeErrors() const {
	return this->error;
}

RPNValueType Analyzer::getLastType() const {
	return this->lastType;
}

void Analyzer::analyze(Line *line) {
	stack = std::stack<AnalyzerValueType>();
	this->currentLine = line;

	for (auto it = line->begin(); it != line->end() && !this->hasErrors(); it++) {
		switch (it->getType()) {
			case TOKEN_TYPE_VALUE:
				stack.push({
					static_cast<const ValueToken *>(*it)->getValueType(),
					it->getRange(),
					false
				});
				break;
			case TOKEN_TYPE_OPERATOR:
			case TOKEN_TYPE_BOOLEAN_OPERATOR:
				this->analyzeOperator(static_cast<const OperatorToken *>(*it));
				break;
			case TOKEN_TYPE_FSTRING:
				this->analyzeFString(static_cast<const FStringToken *>(*it));
				break;
			case TOKEN_TYPE_FUNCTION_CALL:
				this->analyzeFunctionCall(*it);
				break;
			case TOKEN_TYPE_LITERAL:
				stack.push({
					it->getStringValue(),
					it->getRange(),
					true
				});
				break;
			case TOKEN_TYPE_ASSIGNMENT:
				this->analyzeAssignment(*it);
				break;
			case TOKEN_TYPE_VALUE_TYPE:
				this->analyzeTypeCast(static_cast<const TypeToken *>(*it));
				break;
			case TOKEN_TYPE_STRUCT_NAME:
				this->analyzeStructCreation(*it);
				break;
			default:
				throw std::runtime_error("Lexer didn't create a valid token " + Token::stringType(it->getType()));
		}
	}

	if (this->hasErrors()) {
		return;
	}

	if (stack.size() > 1) {
		TextRange range = stack.top().range;
		size_t size = stack.size();
		while (stack.size() > 1) {
			stack.pop();
			range = TextRange::merge(range, stack.top().range);
		}
		this->error = ExpressionResult(
			"To much remaining values in the memory (" + std::to_string(size) + ")",
			range,
			this->context	
		);
	}

	if (stack.size() == 1) {
		this->lastType = this->topVariable().type;
	}
}

AnalyzerValueType& Analyzer::topVariable() {
	if (stack.top().isVariable) {
		AnalyzerValueType top = stack.top();
		stack.pop();
		if (this->variables.find(std::get<std::string>(top.type)) == this->variables.end()) {
			this->error = ExpressionResult(
				"Variable " + std::get<std::string>(top.type) + " is not defined",
				top.range,
				this->context
			);
			this->stack.push(top);
			return stack.top();
		}
		this->stack.push(this->variables[std::get<std::string>(top.type)]);
		this->stack.top().isVariable = false;
	}
	return stack.top();
}

void Analyzer::analyze(FunctionBlock *functionBlock) {
	if (functions.find(functionBlock->getName()) != functions.end()) {
		this->error = ExpressionResult(
			"Function " + functionBlock->getName() + " is already defined",
			functionBlock->lastRange(),
			this->context
		);
		return;
	}
	functions[functionBlock->getName()] = std::pair<RPNFunctionArgTypes, RPNValueType>(
		functionBlock->getArgsTypes(),
		functionBlock->getReturnType()
	);
	functionBlocks.push(functionBlock->getBody());
}

void Analyzer::analyseFunctionsBody() {
	while (!functionBlocks.empty() && !this->hasErrors()) {
		// TODO : create separated variables context
		this->analyze(functionBlocks.front());
		functionBlocks.pop();
	}
}

void Analyzer::analyzeOperator(const OperatorToken *token) {
	if (stack.size() < 2) {
		this->error = ExpressionResult(
			"Not enough values for operator " + token->getStringValue(),
			token->getRange(),
			this->context
		);
		return;
	}
	AnalyzerValueType right = this->topVariable();
	if (this->hasErrors()) return;
	if (right.type.index() == 0) {
		this->error = ExpressionResult(
			"Can't apply operator " + token->getStringValue() + " to struct name",
			token->getRange(),
			this->context
		);
		return;
	}
	stack.pop();
	AnalyzerValueType left = this->topVariable();
	if (this->hasErrors()) return;
	if (left.type.index() == 0) {
		this->error = ExpressionResult(
			"Can't apply operator " + token->getStringValue() + " to struct name",
			token->getRange(),
			this->context
		);
		return;
	}
	stack.pop();
	std::optional<ValueType> resultType = this->getOperatorType(std::get<ValueType>(left.type), std::get<ValueType>(right.type), token);
	if (resultType.has_value()) {
		stack.push({
			resultType.value(),
			TextRange::merge(left.range, right.range).merge(token->getRange()),
			false
		});
	} else {
		this->error = ExpressionResult(
			"Can't apply operator " + token->getStringValue() + " to types " + 
			Value::stringType(std::get<ValueType>(left.type)) + " and " + Value::stringType(std::get<ValueType>(right.type)),
			token->getRange(),
			this->context
		);
	}
}

void Analyzer::analyzeFString(const FStringToken *token) {
	size_t size = token->size() - 1;
	if (stack.size() < size) {
		this->error = ExpressionResult(
			"Not enough values for fstring, expected " + std::to_string(size) + " but got " + std::to_string(this->stack.size()),
			token->getRange(),
			this->context
		);
		return;
	}
	for (size_t i = 0; i < size; i++) {
		stack.pop();
	}
	stack.push({
		STRING,
		token->getRange(),
		false
	});
}

void Analyzer::analyzeFunctionCall(std::pair<RPNFunctionArgTypes, RPNValueType> function, const Token *token) {
	std::string name = token->getStringValue();
	if (this->stack.size() < function.first.size()) {
		this->error = ExpressionResult(
			"Not enough values for function " + name + ", expected " + std::to_string(function.first.size()) + " but got " + std::to_string(this->stack.size()),
			token->getRange(),
			this->context
		);
		return;
	}
	TextRange range = token->getRange();
	RPNValueType type;
	for (int i = function.first.size() - 1; i >= 0; i--) {
		type = this->topVariable().type;
		if (type.index() == 0 && function.first[i].index() != 0) {
			this->error = ExpressionResult(
				"Function " + name + " expects value type " + 
				Value::stringType(std::get<ValueType>(function.first[i])) + 
				" but got struct instead",
				stack.top().range,
				this->context
			);
			return;
		} else if (type.index() != 0 && function.first[i].index() == 0) {
			this->error = ExpressionResult(
				"Function " + name + " expects struct type " + 
				std::get<std::string>(function.first[i]) + 
				" but got value instead",
				stack.top().range,
				this->context
			);
			return;
		} else if (type.index() == 0 && function.first[i].index() == 0) {
			if (std::get<std::string>(type) != std::get<std::string>(function.first[i]))
				this->error = ExpressionResult(
					"Function " + name + " expects struct type " + 
					std::get<std::string>(function.first[i]) + 
					" but got " + std::get<std::string>(type) + " instead",
					stack.top().range,
					this->context
				);
			return;
		} else if (!Value::isCastableTo(std::get<ValueType>(type), std::get<ValueType>(function.first[i]))) {
			this->error = ExpressionResult(
				"Function " + name + " expects value type " + 
				Value::stringType(std::get<ValueType>(function.first[i])) + 
				" but got " + Value::stringType(std::get<ValueType>(type)) + " instead",
				stack.top().range,
				this->context
			);
			return;
		}

		range.merge(stack.top().range);
		stack.pop();
	}
	stack.push({
		function.second,
		range,
		false
	});
}

void Analyzer::analyzeFunctionCall(const Token *token) {
	std::string name = token->getStringValue();
	if (builtins::builtinFunctions.find(name) != builtins::builtinFunctions.end()) {
		this->analyzeFunctionCall(std::make_pair(
			builtins::builtinFunctions.at(name).getArgsTypes(),
			builtins::builtinFunctions.at(name).getReturnType()
		), token);
	} else if (functions.find(name) == functions.end()) {
		this->analyzeFunctionCall(functions[name], token);
	} else {
		this->error = ExpressionResult(
			"Function " + name + " is not defined",
			token->getRange(),
			this->context
		);
		return;
	}
}

void Analyzer::analyzeAssignment(const Token *token) {
	if (stack.size() < 2) {
		this->error = ExpressionResult(
			"Not enough values for assignment, expected 2 but got " + std::to_string(this->stack.size()),
			token->getRange(),
			this->context
		);
		return;
	}
	RPNValueType type = this->topVariable().type;
	stack.pop();
	if (this->hasErrors()) return;
	AnalyzerValueType variable = stack.top();
	if (variable.type.index() != 0 || !variable.isVariable) {
		this->error = ExpressionResult(
			"Can't assign to non-variable",
			variable.range,
			this->context
		);
		return;
	}
	this->variables[std::get<std::string>(variable.type)] = {
		type,
		variable.range,
		false
	};
}

void Analyzer::analyzeTypeCast(const TypeToken *token) {
	if (stack.size() < 1) {
		this->error = ExpressionResult(
			"Not enough values for type cast, expected 1 but got " + std::to_string(this->stack.size()),
			token->getRange(),
			this->context
		);
		return;
	}
	AnalyzerValueType type = this->topVariable();
	stack.pop();
	if (this->hasErrors()) return;
	if (token->getValueType() == LIST && type.type.index() == 1 && std::get<ValueType>(type.type) == INT) {
		return this->analyzeListCreation(token);
	}
	
	if (!Value::isCastableTo(std::get<ValueType>(type.type), token->getValueType())) {
		this->error = ExpressionResult(
			"Can't cast " + Value::stringType(std::get<ValueType>(type.type)) + " to " + Value::stringType(token->getValueType()),
			token->getRange(),
			this->context
		);
		return;
	}
	stack.push({
		token->getValueType(),
		TextRange::merge(token->getRange(), type.range)
	});
}

void Analyzer::analyzeListCreation(const TypeToken *token) {
	int size = static_cast<const Int*>(static_cast<ValueToken*>(this->currentLine->last())->getValue())->getValue();
	if (size < 0) {
		this->error = ExpressionResult(
			"List size can't be negative",
			token->getRange(),
			this->context
		);
		return;
	}
	if (stack.size() < (size_t)size) {
		this->error = ExpressionResult(
			"Not enough values for list creation, expected " + std::to_string(size) + " but got " + std::to_string(this->stack.size()),
			token->getRange(),
			this->context
		);
		return;
	}
	TextRange range = token->getRange();
	for (int i = 0; i < size; i++) {
		range.merge(this->topVariable().range);
		if (this->hasErrors()) return;
		stack.pop();
	}
	stack.push({
		LIST,
		range,
		false
	});
}

void Analyzer::analyzeStructCreation(const Token *token) {
	std::string name = token->getStringValue();
	if (!Struct::structExists(name)) {
		this->error = ExpressionResult(
			"Struct " + name + " is not defined",
			token->getRange(),
			this->context
		);
		return;
	}
	StructDefinition def = Struct::getStructDefinition(name);
	if (stack.size() < def.getMembersCount()) {
		this->error = ExpressionResult(
			"Not enough values for struct creation, expected " + std::to_string(def.getMembersCount()) + " but got " + std::to_string(this->stack.size()),
			token->getRange(),
			this->context
		);
		return;
	}

	TextRange range = token->getRange();
	std::vector<std::string> members = def.getMembersOrder();
	for (auto it = members.end() - 1; it >= members.begin(); it--) {
		RPNValueType type = def.getMemberType(*it);
		this->topVariable();
		if (type.index() == 0) {
			if (stack.top().type.index() != 0) {
				this->error = ExpressionResult(
					"Struct member " + *it + " is of type " + std::get<std::string>(type) + " but got " + Value::stringType(std::get<ValueType>(stack.top().type)),
					stack.top().range,
					this->context
				);
				return;
			}
			if (std::get<std::string>(type) != std::get<std::string>(stack.top().type)) {
				this->error = ExpressionResult(
					"Struct member " + *it + " is of type " + std::get<std::string>(type) + " but got " + std::get<std::string>(stack.top().type),
					stack.top().range,
					this->context
				);
				return;
			}
		} else {
			if (stack.top().type.index() != 1) {
				this->error = ExpressionResult(
					"Struct member " + *it + " is of type " + Value::stringType(std::get<ValueType>(type)) + " but got " + std::get<std::string>(stack.top().type),
					stack.top().range,
					this->context
				);
				return;
			}
			if (std::get<ValueType>(type) != std::get<ValueType>(stack.top().type)) {
				this->error = ExpressionResult(
					"Struct member " + *it + " is of type " + Value::stringType(std::get<ValueType>(type)) + " but got " + Value::stringType(std::get<ValueType>(stack.top().type)),
					stack.top().range,
					this->context
				);
				return;
			}
		}
		range.merge(stack.top().range);
		stack.pop();
	}
	stack.push({
		name,
		range,
		false
	});
}

bool Analyzer::isBinaryOperator(OperatorToken::OperatorTypes operatorType) {
	return  operatorType == OperatorToken::OP_ADD ||
			operatorType == OperatorToken::OP_SUB ||
			operatorType == OperatorToken::OP_MUL ||
			operatorType == OperatorToken::OP_DIV ||
			operatorType == OperatorToken::OP_MOD;
}

bool Analyzer::isComparisonOperator(OperatorToken::OperatorTypes operatorType) {
	return  operatorType == OperatorToken::OP_EQ || 
			operatorType == OperatorToken::OP_NE;
}

std::optional<ValueType> Analyzer::getOperatorType(ValueType left, ValueType right, const OperatorToken *operatorToken) {
	OperatorToken::OperatorTypes operatorType = operatorToken->getOperatorType();
	// for now, there is no way to différentiate return value of a function with ANY type as return value
	if (left == ANY || right == ANY) {
		return ANY;
	}

	if (left == LIST) {
		if (operatorType == OperatorToken::OP_ADD && right == LIST) {
			return LIST;
		} else if (operatorType == OperatorToken::OP_MUL && right == INT) {
			return LIST;
		} else if (Analyzer::isComparisonOperator(operatorType)) {
			return BOOL;
		}
		return std::nullopt;
	} else if (right == LIST) {
		if (operatorType == OperatorToken::OP_ADD && right == LIST) {
			return LIST;
		} else if (Analyzer::isComparisonOperator(operatorType)) {
			return BOOL;
		}
		return std::nullopt;
	}

	if (left == STRING) {
		if (operatorType == OperatorToken::OP_ADD && right == STRING) {
			return STRING;
		} else if (operatorType == OperatorToken::OP_MUL && right == INT) {
			return STRING;
		} else if (Analyzer::isComparisonOperator(operatorType)) {
			return BOOL;
		}
		return std::nullopt;
	} else if (right == STRING) {
		if (operatorType == OperatorToken::OP_ADD && right == STRING) {
			return STRING;
		} else if (Analyzer::isComparisonOperator(operatorType)) {
			return BOOL;
		}
		return std::nullopt;
	}

	if (Analyzer::isBinaryOperator(operatorType)) {
		if ((left == INT || left == BOOL) && (right == INT || right == BOOL)) {
			return INT;
		} else if ((left == INT || left == BOOL) && right == FLOAT) {
			return FLOAT;
		} else if (left == FLOAT && (right == INT || right == BOOL)) {
			return FLOAT;
		} else if (left == FLOAT && right == FLOAT) {
			return FLOAT;
		}
	} else if (Analyzer::isComparisonOperator(operatorType)) {
		return BOOL;
	} else if (
		operatorType == OperatorToken::OP_GT ||
		operatorType == OperatorToken::OP_GE ||
		operatorType == OperatorToken::OP_LT ||
		operatorType == OperatorToken::OP_LE
	) {
		if ((left == BOOL || left == INT || left == FLOAT) && (right == BOOL || right == INT || right == FLOAT)) {
			return BOOL;
		}
	}

	return std::nullopt;
}