#include "analyzer/analyzer.hpp"

Analyzer::Analyzer(ContextPtr context) : context(context), conditionalLevel(0), inFunctionBlock(false) {}

void Analyzer::analyze(BlockQueue &blocks, bool entryPoint) {
	for (auto it = blocks.begin(); it != blocks.end() && !this->hasErrors(); it++) {
		if ((*it)->getType() == LINE_BLOCK) {
			this->analyze(static_cast<Line *>(*it));
			if (this->hasErrors()) continue;
			if (it != blocks.end() - (size_t)1 && (*(it + (size_t)1))->getType() == CODE_BLOCK) {
				this->checkKeywordLine(static_cast<CodeBlock *>(*(it + (size_t)1))->getKeywordToken());
			} else {
				this->checkRemainingCount();
			}
		} else if ((*it)->getType() == CODE_BLOCK) {
			this->analyze(static_cast<CodeBlock *>(*it));
		} else if ((*it)->getType() == FUNCTION_BLOCK) {
			this->analyze(static_cast<FunctionBlock *>(*it));
		} else {
			throw std::runtime_error("This block type is not supported");
		}
	}
	if (entryPoint) {
		this->analyzeFunctionsBody();
	}
}

void Analyzer::analyze(CodeBlock *block) {
	this->conditionalLevel++;
	this->nextConditionalLevel = 1;
	this->analyze(block->getBlocks());
	while ((block = block->getNext()) && !this->hasErrors()) {
		this->nextConditionalLevel++;
		this->analyze(block->getBlocks());
	}
	this->conditionalLevel--;
	if (this->nextConditionalLevel == 1) return;
	for (auto &variable : this->variables) {
		if (variable.second.conditionalNextLevel == this->nextConditionalLevel) {
			variable.second.conditionalLevel = this->conditionalLevel;
		}
	}
}

bool Analyzer::hasErrors() const {
	return this->error.error();
}

ExpressionResult Analyzer::analyzeErrors() const {
	return this->error;
}

void Analyzer::checkRemainingCount() {
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
	if (this->stack.empty()) return;
	if (!this->stack.top().isVariable) return;
	std::string name = std::get<std::string>(this->stack.top().type);
	if (this->variables.find(name) == this->variables.end()) {
		this->error = ExpressionResult(
			"Variable '" + name + "' is not defined",
			this->stack.top().range,
			this->context
		);
	}
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
			case TOKEN_TYPE_KEYWORD:
				this->analyzeKeyword(static_cast<const KeywordToken *>(*it));
				break;
			default:
				throw std::runtime_error("Lexer didn't create a valid token : " + Token::stringType(it->getType()));
		}
		lastToken = *it;
	}
}

AnalyzerValueType& Analyzer::topVariable() {
	if (stack.top().isVariable) {
		AnalyzerValueType top = stack.top();
		TextRange range = top.range;
		stack.pop();
		std::unordered_map<std::string, AnalyzerValueType> *variablesMap = &this->functionVariables;
		if (this->functionVariables.find(std::get<std::string>(top.type)) == this->variables.end()) {
			variablesMap = &this->variables;
			if (this->variables.find(std::get<std::string>(top.type)) == this->variables.end()) {
				this->error = ExpressionResult(
					"Variable " + std::get<std::string>(top.type) + " is not defined",
					top.range,
					this->context
				);
				this->stack.push(top);
				return stack.top();
			}
		}
		if (variablesMap->at(std::get<std::string>(top.type)).conditionalLevel > this->conditionalLevel) {
			this->error = ExpressionResult(
				"Variable " + std::get<std::string>(top.type) + 
				" has been declared conditionally and cannot be used outside of the condition scope",
				top.range,
				this->context
			);
			this->stack.push(top);
			return stack.top();
		}
		this->stack.push(variablesMap->at(std::get<std::string>(top.type)));
		this->stack.top().isVariable = false;
		this->stack.top().range = range;
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
	std::vector<RPNValueType> argsTypes;
	for (auto valuesType : functionBlock->getArgs())
		argsTypes.push_back(valuesType.second);
	
	functions[functionBlock->getName()] = std::pair<std::vector<RPNValueType>, RPNValueType>(
		argsTypes,
		functionBlock->getReturnType()
	);
	functionBlocks.push(functionBlock);
}

void Analyzer::analyzeFunctionsBody() {
	while (!this->functionBlocks.empty() && !this->hasErrors()) {
		RPNFunctionArgs args = this->functionBlocks.top()->getArgs();
		for (const auto &arg : args) {
			this->functionVariables[arg.first] = {
				arg.second,
				this->functionBlocks.top()->lastRange(),
				true,
				0
			};
		}
		this->analyze(this->functionBlocks.top()->getBlocks());
		this->functionBlocks.pop();
		this->functionVariables.clear();
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

void Analyzer::analyzeFunctionCall(std::pair<std::vector<RPNValueType>, RPNValueType> function, const Token *token) {
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
	if (functions.find(name) != functions.end()) {
		this->analyzeFunctionCall(functions[name], token);
	} else if (builtins::builtinFunctions.find(name) != builtins::builtinFunctions.end()) {
		std::vector<RPNValueType> argsTypes;
		for (auto valuesType : builtins::builtinFunctions.at(name).getArgs())
			argsTypes.push_back(valuesType.second);
		
		auto function = std::make_pair(
			argsTypes,
			builtins::builtinFunctions.at(name).getReturnType()
		);
		this->functions[name] = function;
		this->analyzeFunctionCall(function, token);
	} else if (functions.find(name) != functions.end()) {
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
	std::string name = std::get<std::string>(variable.type);
	if (this->conditionalLevel > 0 && this->variables.find(name) != this->variables.end()) {
		// check if the type is the same or equivalent
		AnalyzerValueType holdVariable = this->variables.at(name);
		if (holdVariable.type.index() == 0) {
			if (type.index() != 0) {
				this->error = ExpressionResult(
					"Can't conditionally assign value of type " + Value::stringType(std::get<ValueType>(type)) + 
					" to variable " + name + " of type " + std::get<std::string>(holdVariable.type),
					variable.range,
					this->context
				);
				return;
			}
			if (std::get<std::string>(type) != std::get<std::string>(holdVariable.type)) {
				this->error = ExpressionResult(
					"Can't conditionally assign value of type " + std::get<std::string>(type) + 
					" to variable " + name + " of type " + std::get<std::string>(holdVariable.type),
					variable.range,
					this->context
				);
				return;
			}
		}
		if (
			!Value::isCastableTo(std::get<ValueType>(type),std::get<ValueType>(holdVariable.type)) ||
			!Value::isCastableTo(std::get<ValueType>(holdVariable.type), std::get<ValueType>(type))
		) {
			this->error = ExpressionResult(
				"Can't conditionally assign value of type " + Value::stringType(std::get<ValueType>(type)) + 
				" to variable " + name + " of type " + Value::stringType(std::get<ValueType>(holdVariable.type)),
				variable.range,
				this->context
			);
			return;
		}
		this->variables[name] = {
			type,
			variable.range,
			true,
			holdVariable.conditionalLevel,
			this->nextConditionalLevel - 1 == holdVariable.conditionalLevel ? holdVariable.conditionalLevel + 1 : holdVariable.conditionalLevel
		};
		return;
	}
	this->variables[name] = {
		type,
		variable.range,
		false,
		this->conditionalLevel,
		this->nextConditionalLevel == 1 ? 1 : 0
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
	int size = static_cast<const Int*>(static_cast<ValueToken*>(this->lastToken)->getValue())->getValue();
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
			"Not enough values for struct creation, expected " + 
			std::to_string(def.getMembersCount()) + " but got " + std::to_string(this->stack.size()),
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

void Analyzer::analyzeKeyword(const KeywordToken *token) {
	switch (token->getKeyword()) {
		case KEYWORD_BREAK:
		case KEYWORD_CONTINUE:
			if (this->stack.size() != 0) {
				this->error = ExpressionResult(
					"Break and continue can't have any value",
					token->getRange(),
					this->context
				);
				return;
			}
			break;
		case KEYWORD_RETURN:
			if (this->stack.size() > 1) {
				this->error = ExpressionResult(
					"Return can't have more than one value",
					token->getRange(),
					this->context
				);
				return;
			}
			if (this->stack.size() == 0) {
				this->error = ExpressionResult(
					"Return must have a value",
					token->getRange(),
					this->context
				);
			}
			break;
		default:
			throw std::runtime_error("The keyword " + token->getStringValue() + " is not implemented");
	}
}

void Analyzer::checkKeywordLine(const KeywordToken *token) {
	if (linePatern.find(token->getKeyword()) == linePatern.end()) {
		this->checkRemainingCount();
	}
	std::vector<RPNValueType> types = linePatern.at(token->getKeyword());
	if (types.size() != this->stack.size()) {
		this->error = ExpressionResult(
			"Keyword " + token->getStringValue() + " require " + 
			std::to_string(types.size()) + " values but got " + std::to_string(this->stack.size()),
			token->getRange(),
			this->context
		);
		return;
	}
	for (int i = types.size() - 1; i >= 0; i--) {
		if (types[i].index() == 0) {
			if (stack.top().type.index() != 0) {
				this->error = ExpressionResult(
					"Keyword " + token->getStringValue() + " require a value of type " + 
					std::get<std::string>(types[i]) + " but got " + 
					Value::stringType(std::get<ValueType>(stack.top().type)),
					stack.top().range,
					this->context
				);
				return;
			}
			if (std::get<std::string>(types[i]) != std::get<std::string>(stack.top().type)) {
				this->error = ExpressionResult(
					"Keyword " + token->getStringValue() + " require a value of type " + 
					std::get<std::string>(types[i]) + " but got " + std::get<std::string>(stack.top().type),
					stack.top().range,
					this->context
				);
				return;
			}
		} else {
			if (std::get<ValueType>(types[i]) == VARIABLE) {
				if (stack.top().type.index() != 0) {
					this->error = ExpressionResult(
						"Keyword " + token->getStringValue() + " require a value of type " + 
						Value::stringType(std::get<ValueType>(types[i])) + " but got " + 
						Value::stringType(std::get<ValueType>(stack.top().type)),
						stack.top().range,
						this->context
					);
					return;
				}
				if (!stack.top().isVariable) {
					this->error = ExpressionResult(
						"Keyword " + token->getStringValue() + " require a value of type " + 
						Value::stringType(std::get<ValueType>(types[i])) + " but got " + 
						std::get<std::string>(stack.top().type),
						stack.top().range,
						this->context
					);
					return;
				}
				this->variables[std::get<std::string>(stack.top().type)] = {
					INT,
					stack.top().range,
					false
				};
				continue;
			}
			this->topVariable();
			if (stack.top().type.index() != 1) {
				this->error = ExpressionResult(
					"Keyword " + token->getStringValue() + " require a value of type " + Value::stringType(std::get<ValueType>(types[i])) + " but got " + std::get<std::string>(stack.top().type),
					stack.top().range,
					this->context
				);
				return;
			}
			if (std::get<ValueType>(types[i]) != std::get<ValueType>(stack.top().type)) {
				this->error = ExpressionResult(
					"Keyword " + token->getStringValue() + " require a value of type " + Value::stringType(std::get<ValueType>(types[i])) + " but got " + Value::stringType(std::get<ValueType>(stack.top().type)),
					stack.top().range,
					this->context
				);
				return;
			}
		}
		stack.pop();
	}
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
		if (operatorType == OperatorToken::OP_ADD && left == STRING) {
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