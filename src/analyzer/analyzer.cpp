#include "analyzer/analyzer.hpp"

Analyzer::Analyzer(ContextPtr context) : context(context) {
	// add variables of the current context
	for (const auto &[name, variable] : context->getSymbols()) {
		this->variables[name] = {
			variable->getType(), variable->getRange(), false, 0, 0, false, false};
		if (variable->getType() == FUNCTION) {
			this->functions[name] = dynamic_cast<Function *>(variable)->getValue()->getSignature();
		}
	}
}

void Analyzer::analyze(BlockQueue &blocks, bool entryPoint) {
	for (auto it = blocks.begin(); it != blocks.end() && !this->hasErrors(); it++) {
		if ((*it)->getType() == blockType::LINE_BLOCK) {
			this->analyze(dynamic_cast<Line *>(*it));
			if (this->hasErrors()) {
				continue;
			}
			if (it != blocks.end() - (size_t)1 &&
				(*(it + (size_t)1))->getType() == blockType::CODE_BLOCK) {
				this->checkKeywordLine(
					dynamic_cast<CodeBlock *>(*(it + (size_t)1))->getKeywordToken(), false);
			} else {
				this->checkRemainingCount();
			}
		} else if ((*it)->getType() == blockType::CODE_BLOCK) {
			this->analyze(dynamic_cast<CodeBlock *>(*it));
		} else if ((*it)->getType() == blockType::FUNCTION_BLOCK) {
			this->analyze(dynamic_cast<FunctionBlock *>(*it));
		} else if ((*it)->getType() == blockType::FUNCTION_SIGNATURE) {
			this->analyze(dynamic_cast<FunctionSignatureLine *>(*it));
		} else {
			throw std::invalid_argument("This block type is not supported");
		}
	}
	if (entryPoint) {
		this->analyzeFunctionsBody();
	}
}

void Analyzer::analyze(CodeBlock *block) {
	this->conditionalLevel++;
	if (this->nextConditionalLevel.size() <= this->conditionalLevel) {
		this->nextConditionalLevel.push_back(1);
	} else {
		this->nextConditionalLevel[this->conditionalLevel] = 1;
	}
	this->analyze(block->getBlocks());
	while (((block = block->getNext()) != nullptr) && !this->hasErrors()) {
		this->nextConditionalLevel[this->conditionalLevel]++;
		this->analyze(block->getBlocks());
	}

	if (this->nextConditionalLevel[this->conditionalLevel] != 1) {
		for (auto &[_, variable] : this->variables) {
			if (variable.conditionalNextLevel ==
				this->nextConditionalLevel[this->conditionalLevel]) {
				variable.conditionalLevel = this->conditionalLevel - 1;
			}
		}
	}
	this->conditionalLevel--;
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
		size_t const size = stack.size();
		while (stack.size() > 1) {
			stack.pop();
			range = TextRange::merge(range, stack.top().range);
		}
		this->error = ExpressionResult("To much remaining values in the memory (" +
										   std::to_string(size) + ")",
									   range, this->context);
	}
	if (this->stack.empty()) {
		return;
	}
	if (!this->stack.top().isVariable) {
		return;
	}
	this->topVariable();
}

void Analyzer::analyze(Line *line) {
	stack = std::stack<AnalyzerValueType>();
	this->currentLine = line;
	Token *token = nullptr;
	while (!this->currentLine->empty() && !this->hasErrors()) {
		token = this->currentLine->top();
		switch (token->getType()) {
			case TokenType::TOKEN_TYPE_VALUE:
				this->stack.emplace(dynamic_cast<const ValueToken *>(token)->getValueType(),
									token->getRange(), false, 0, 0, false);
				break;
			case TokenType::TOKEN_TYPE_OPERATOR:
			case TokenType::TOKEN_TYPE_BOOLEAN_OPERATOR:
				this->analyzeOperator(dynamic_cast<const OperatorToken *>(token));
				break;
			case TokenType::TOKEN_TYPE_FSTRING:
				this->analyzeFString(dynamic_cast<const FStringToken *>(token));
				break;
			case TokenType::TOKEN_TYPE_FUNCTION_CALL:
			case TokenType::TOKEN_TYPE_MODULE_FUNCTION_CALL:
				this->analyzeFunctionCall(token);
				break;
			case TokenType::TOKEN_TYPE_LITERAL:
				if (builtins::builtinFunctions.contains(token->getStringValue())) {
					dynamic_cast<ValueToken *>(token)->getValue()->setType(BUILTIN_VARIABLE);
				}
				this->stack.emplace(token->getStringValue(), token->getRange(), true, 0, 0, false);
				break;
			case TokenType::TOKEN_TYPE_ASSIGNMENT:
				this->analyzeAssignment(token);
				break;
			case TokenType::TOKEN_TYPE_VALUE_TYPE:
				this->analyzeTypeCast(dynamic_cast<const TypeToken *>(token));
				break;
			case TokenType::TOKEN_TYPE_STRUCT_NAME:
				this->analyzeStructCreation(token);
				break;
			case TokenType::TOKEN_TYPE_KEYWORD:
				this->analyzeKeyword(dynamic_cast<const KeywordToken *>(token));
				break;
			case TokenType::TOKEN_TYPE_PATH:
				this->analyzePath(token);
				break;
			case TokenType::TOKEN_TYPE_STRUCT_ACCESS:
				this->analyzeStructAccess(token);
				break;
			case TokenType::TOKEN_TYPE_FUN_SIGNATURE:
				this->error = {
					"Function signature can't be used in this context",
					token->getRange(),
					this->context
				};
				break;
			default:
				throw std::runtime_error("Lexer didn't create a valid token : " +
										 Token::stringType(token->getType()));
		}
		this->currentLine->pop();
	}
	line->reset();
}

std::optional<AnalyzerValueType> Analyzer::getVariable(const std::string &name) {
	AnalyzerSymbolTable *variablesMap = nullptr;
	if (this->functionVariables.contains(name)) {
		variablesMap = &this->functionVariables;
	} else if (this->variables.contains(name)) {
		variablesMap = &this->variables;
	} else if (builtins::builtinFunctions.contains(name)) {
		BuiltinRPNFunction const &builtinFunction = builtins::builtinFunctions.at(name);
		this->functions[name] = builtinFunction.getSignature();
		AnalyzerValueType type = {name, TextRange(), false, 0, 0, false, false, true};
		this->variables[name] = type;
		return type;
	} else {
		return std::nullopt;
	}

	return {variablesMap->at(name)};
}

AnalyzerValueType &Analyzer::topVariable() {
	if (stack.top().isVariable) {
		AnalyzerValueType top = stack.top();
		TextRange const range = top.range;
		stack.pop();
		auto variable = this->getVariable(std::get<std::string>(top.type.getType()));
		if (!variable.has_value()) {
			this->error = ExpressionResult("Variable " + top.type.name() + " is not defined", range,
										   this->context);
			stack.push(top);
			return stack.top();
		}
		AnalyzerValueType &variableValue = variable.value();
		if (variableValue.conditionalLevel > this->conditionalLevel) {
			this->error = ExpressionResult("Variable " + top.type.name() +
											   " has been declared conditionally and cannot be "
											   "used outside of the condition scope",
										   top.range, this->context);
			this->stack.push(top);
			return stack.top();
		}
		this->stack.push(variableValue);
		this->stack.top().isVariable = false;
		this->stack.top().range = range;
	}
	return stack.top();
}

AnalyzerSymbolTable *Analyzer::getVariables() {
	return this->inFunctionBlock ? &this->functionVariables : &this->variables;
}

void Analyzer::analyze(FunctionBlock *functionBlock) {
	if (functions.contains(functionBlock->getName())) {
		this->error =
			ExpressionResult("Function " + functionBlock->getName() + " is already defined",
							 functionBlock->lastRange(), this->context);
		return;
	}

	functions[functionBlock->getName()] = functionBlock->getSignature();
	variables[functionBlock->getName()] = {
		functionBlock->getName(), functionBlock->lastRange(), false, 0, 0, false, false, true};
	functionBlocks.push(functionBlock);
}

void Analyzer::analyze(const FunctionSignatureLine *functionSignatureLine) {
	if (functions.contains(functionSignatureLine->getName())) {
		this->error =
			ExpressionResult("Function " + functionSignatureLine->getName() + " is already defined",
							 functionSignatureLine->lastRange(), this->context);
		return;
	}

	functions[functionSignatureLine->getName()] = functionSignatureLine->getSignature();
}

void Analyzer::analyzeFunctionsBody() {
	this->inFunctionBlock = true;
	while (!this->functionBlocks.empty() && !this->hasErrors()) {
		RPNFunctionArgs const args = this->functionBlocks.top()->getArgs();
		for (const auto &[name, valueType] : args) {
			if (valueType.index() == 0 && std::get<std::string>(valueType.getType())[0] == '$') {
				const std::string functionName =
					std::get<std::string>(valueType.getType()).substr(1);
				if (!this->functions.contains(functionName)) {
					this->error =
						ExpressionResult("Function signature " + functionName + " is not defined",
										 this->functionBlocks.top()->lastRange(), this->context);
					return;
				}
				this->functionVariables[name] = {functionName,
												 this->functionBlocks.top()->lastRange(),
												 false,
												 0,
												 0,
												 false,
												 false,
												 true};
			} else {
				this->functionVariables[name] = {valueType, this->functionBlocks.top()->lastRange(),
												 true, 0};
			}
		}
		this->currentFunctionReturnType = this->functionBlocks.top()->getReturnType();
		this->analyze(this->functionBlocks.top()->getBlocks());
		this->functionBlocks.pop();
		this->functionVariables.clear();
	}
	this->inFunctionBlock = false;
}

void Analyzer::analyzeOperator(const OperatorToken *token) {
	if (stack.size() < 2) {
		this->error = ExpressionResult("Not enough values for operator " + token->getStringValue(),
									   token->getRange(), this->context);
		return;
	}
	AnalyzerValueType right = this->topVariable();
	if (this->hasErrors()) {
		return;
	}
	if (right.type.index() == 0) {
		this->error =
			ExpressionResult("Can't apply operator " + token->getStringValue() + " to struct name",
							 token->getRange(), this->context);
		return;
	}
	stack.pop();
	AnalyzerValueType left = this->topVariable();
	if (this->hasErrors()) {
		return;
	}
	if (left.type.index() == 0) {
		this->error =
			ExpressionResult("Can't apply operator " + token->getStringValue() + " to struct name",
							 token->getRange(), this->context);
		return;
	}
	stack.pop();
	std::optional<ValueType> resultType = Analyzer::getOperatorType(
		std::get<ValueType>(left.type.getType()), std::get<ValueType>(right.type.getType()), token);
	if (resultType.has_value()) {
		if (std::get<ValueType>(left.type.getType()) == LIST &&
			std::get<ValueType>(right.type.getType()) == LIST) {
			std::optional<ValueType> listType =
				Analyzer::getOperatorType(std::get<ValueType>(left.type.getListType()),
										  std::get<ValueType>(right.type.getListType()), token);
			if (listType.has_value()) {
				stack.emplace(RPNValueType{LIST, listType.value()},
							  TextRange::merge(left.range, right.range).merge(token->getRange()),
							  false);
				return;
			}
		} else if (std::get<ValueType>(left.type.getType()) == LIST) {
			stack.emplace(RPNValueType{LIST, std::get<ValueType>(left.type.getListType())},
						  TextRange::merge(left.range, right.range).merge(token->getRange()),
						  false);
			return;
		} else if (std::get<ValueType>(right.type.getType()) == LIST) {
			stack.emplace(RPNValueType{LIST, std::get<ValueType>(right.type.getListType())},
						  TextRange::merge(left.range, right.range).merge(token->getRange()),
						  false);
			return;
		}
		stack.emplace(resultType.value(),
					  TextRange::merge(left.range, right.range).merge(token->getRange()), false);
		return;
	}

	this->error =
		ExpressionResult("Can't apply operator " + token->getStringValue() + " to types " +
							 left.type.name() + " and " + right.type.name(),
						 token->getRange(), this->context);
}

void Analyzer::analyzeFString(const FStringToken *token) {
	size_t const size = token->size() - 1;
	if (stack.size() < size) {
		this->error =
			ExpressionResult("Not enough values for fstring, expected " + std::to_string(size) +
								 " but got " + std::to_string(this->stack.size()),
							 token->getRange(), this->context);
		return;
	}
	for (size_t i = 0; i < size; i++) {
		stack.pop();
	}
	stack.emplace(STRING, token->getRange(), false);
}

void Analyzer::analyzeFunctionCall(FunctionSignature function, Token *token) {
	if (dynamic_cast<ValueToken *>(token)->getValueType() == PATH) {
		dynamic_cast<ValueToken *>(token)->getValue()->setType(function.builtin ? BUILTIN_PATH
																				: PATH);
	}

	std::string const name = token->getStringValue();
	if (this->stack.size() < function.args.size()) {
		this->error = ExpressionResult("Not enough values for function " + name + ", expected " +
										   std::to_string(function.args.size()) + " but got " +
										   std::to_string(this->stack.size()),
									   token->getRange(), this->context);
		return;
	}
	TextRange range = token->getRange();
	RPNValueType type;
	AnalyzerValueType top;
	for (int i = function.args.size() - 1; i >= 0; i--) {
		top = this->topVariable();
		type = top.type;
		if (this->hasErrors()) {
			return;
		}
		const std::string typeName = type.name();
		const std::string argName = function.args[i].name();
		if (top.isFunction && argName[0] == '$') {
			if (!this->checkFunctionSignature(std::string_view(argName).substr(1), typeName)) {
				this->error =
					ExpressionResult("Function " + name + " expects a function with signature " +
										 argName + " but got " + typeName,
									 stack.top().range, this->context);
			}
		} else if ((function.args[i].index() == 0 ||
					std::get<ValueType>(function.args[i].getType()) != ANY) &&
				   type != function.args[i]) {
			this->error = ExpressionResult("Function " + name + " expects value type " + argName +
											   " but got " + typeName,
										   stack.top().range, this->context);
			return;
		}
		range.merge(stack.top().range);
		stack.pop();
	}
	this->stack.emplace(function.returnType, range, false);
}

std::optional<FunctionSignature> Analyzer::checkBuiltinFunction(Token *token) {
	const RPNFunction *function = nullptr;
	const std::string name = token->getStringValue();
	bool builtin = false;
	if (token->getType() == TokenType::TOKEN_TYPE_MODULE_FUNCTION_CALL) {
		this->analyzePath(token, false);
		if (this->hasErrors()) {
			return std::nullopt;
		}
		const Value *path = dynamic_cast<const ValueToken *>(token)->getValue();
		function = dynamic_cast<const Function *>(Module::getModuleValue(path))->getValue();
		builtin = path->getType() == BUILTIN_PATH;
	} else if (builtins::builtinFunctions.contains(name)) {
		function = &builtins::builtinFunctions.at(name);
		builtin = true;
	}
	if (function == nullptr) {
		return std::nullopt;
	}

	FunctionSignature functionValue = function->getSignature();
	functionValue.builtin = builtin;
	this->functions[name] = functionValue;
	this->variables[name] = {name, TextRange(), false, 0, 0, false, false, true};
	return functionValue;
}

void Analyzer::analyzeFunctionCall(Token *token) {
	std::string const name = token->getStringValue();
	if (functions.contains(name)) {
		this->analyzeFunctionCall(functions[name], token);
		return;
	}
	const auto variable = this->getVariable(name);
	if (variable.has_value()) {
		const AnalyzerValueType &variableValue = variable.value();
		if (variableValue.type.index() != 0 || variableValue.isFunction != true) {
			this->error = ExpressionResult("Variable " + name + " is not a function",
										   token->getRange(), this->context);
			return;
		}
		const std::string functionName = std::get<std::string>(variableValue.type.getType());
		if (functions.contains(functionName)) {
			this->analyzeFunctionCall(functions[functionName], token);
			return;
		}
	}
	auto functionValue = this->checkBuiltinFunction(token);
	if (!functionValue.has_value()) {
		if (this->hasErrors()) {
			return;
		}
		this->error = ExpressionResult("Function " + name + " is not defined", token->getRange(),
									   this->context);
		return;
	}
	this->analyzeFunctionCall(functionValue.value(), token);
}

bool Analyzer::checkFunctionSignature(const std::string_view &signatureName,
									  const std::string_view &functionnName) {
	if (signatureName == functionnName) {
		return true;
	}
	const FunctionSignature &expectedSignature = this->functions.at(std::string(signatureName));
	const FunctionSignature &actualSignature = this->functions.at(std::string(functionnName));
	if (expectedSignature.returnType.getType() != actualSignature.returnType.getType()) {
		return false;
	}
	if (expectedSignature.args.size() != actualSignature.args.size()) {
		return false;
	}
	for (size_t i = 0; i < expectedSignature.args.size(); i++) {
		if (expectedSignature.args[i].getType() != actualSignature.args[i].getType()) {
			return false;
		}
	}
	return true;
}

void Analyzer::analyzeAssignment(const Token *token) {
	if (stack.size() < 2) {
		this->error = ExpressionResult("Not enough values for assignment, expected 2 but got " +
										   std::to_string(this->stack.size()),
									   token->getRange(), this->context);
		return;
	}
	RPNValueType type = this->topVariable().type;
	stack.pop();
	if (this->hasErrors()) {
		return;
	}
	AnalyzerValueType variable = stack.top();
	if (variable.isStructMember) {
		if (this->stack.top().type != type) {
			this->error = ExpressionResult("Struct member is of type " + this->stack.top().name() +
											   " but value is of type " + type.name(),
										   this->stack.top().range, this->context);
		}
		return;
	}
	if (variable.isListElement) {
		if (this->stack.top().type != type) {
			this->error = ExpressionResult("List element is of type " + this->stack.top().name() +
											   " but value is of type " + type.name(),
										   this->stack.top().range, this->context);
		}
		return;
	}
	if (variable.type.index() != 0 || !variable.isVariable) {
		this->error =
			ExpressionResult("Can't assign to non-variable", variable.range, this->context);
		return;
	}
	AnalyzerSymbolTable *variables = this->getVariables();
	std::string const name = std::get<std::string>(variable.type.getType());
	auto holdVariableVariant = getVariable(name);
	if (holdVariableVariant.has_value()) {
		AnalyzerValueType holdVariable = holdVariableVariant.value();
		if (holdVariable.type != type || type != holdVariable.type) {
			std::string typeName = holdVariable.type.name();
			if (holdVariable.isFunction) {
				typeName = "FUNCTION";
			}
			this->error = ExpressionResult("Can't assign value of type " + type.name() +
											   " to variable " + name + " of type " + typeName,
										   variable.range, this->context);
			return;
		}
		if (this->conditionalLevel > 0) {
			// check if the type is the same or equivalent
			if (this->conditionalLevel > holdVariable.conditionalLevel) {
				(*variables)[name] = {type, variable.range, true, holdVariable.conditionalLevel,
									  holdVariable.conditionalNextLevel};
				return;
			}
			(*variables)[name] = {type, variable.range, true, holdVariable.conditionalLevel,
								  this->nextConditionalLevel.at(this->conditionalLevel) - 1 ==
										  holdVariable.conditionalNextLevel
									  ? holdVariable.conditionalNextLevel + 1
									  : holdVariable.conditionalNextLevel};
			return;
		}
	}
	(*variables)[name] = {type, variable.range, false, this->conditionalLevel,
						  this->nextConditionalLevel.at(this->conditionalLevel) == 1 ? 1U : 0U};
}

void Analyzer::analyzeTypeCast(const TypeToken *token) {
	if (stack.empty()) {
		this->error = ExpressionResult("Not enough values for type cast, expected 1 but got " +
										   std::to_string(this->stack.size()),
									   token->getRange(), this->context);
		return;
	}
	AnalyzerValueType type = this->topVariable();
	stack.pop();
	if (this->hasErrors()) {
		return;
	}
	if (std::get<ValueType>(token->getValueType().getType()) == LIST && type.type.index() == 1 &&
		std::get<ValueType>(type.type.getType()) == INT) {
		return this->analyzeListCreation(token);
	}

	if (type.type != token->getValueType()) {
		this->error =
			ExpressionResult("Can't cast " + type.name() + " to " + token->getValueType().name(),
							 token->getRange(), this->context);
		return;
	}
	stack.emplace(token->getValueType(), TextRange::merge(token->getRange(), type.range));
}

void Analyzer::analyzeListCreation(const TypeToken *token) {
	int64_t const size =
		dynamic_cast<const Int *>(dynamic_cast<ValueToken *>(this->currentLine->last())->getValue())
			->getValue();
	if (size < 0) {
		this->error =
			ExpressionResult("List size can't be negative", token->getRange(), this->context);
		return;
	}
	if (stack.size() < (size_t)size) {
		this->error = ExpressionResult("Not enough values for list creation, expected " +
										   std::to_string(size) + " but got " +
										   std::to_string(this->stack.size()),
									   token->getRange(), this->context);
		return;
	}
	TextRange range = token->getRange();
	auto listType = token->getListType();
	for (int i = 0; i < size; i++) {
		range.merge(this->topVariable().range);
		if (this->hasErrors()) {
			return;
		}
		if (this->stack.top().type != listType) {
			this->error = ExpressionResult("List is of type " + RPNValueType::typeName(listType) +
											   " but got " + this->stack.top().name(),
										   this->stack.top().range, this->context);
			return;
		}
		stack.pop();
	}
	stack.emplace(RPNValueType(LIST, listType), range, false);
}

void Analyzer::analyzeStructCreation(const Token *token) {
	std::string const name = token->getStringValue();
	if (!Struct::structExists(name)) {
		this->error = ExpressionResult("Struct " + name + " is not defined", token->getRange(),
									   this->context);
		return;
	}
	StructDefinition const def = Struct::getStructDefinition(name);
	if (stack.size() < def.getMembersCount()) {
		this->error = ExpressionResult("Not enough values for struct creation, expected " +
										   std::to_string(def.getMembersCount()) + " but got " +
										   std::to_string(this->stack.size()),
									   token->getRange(), this->context);
		return;
	}

	TextRange range = token->getRange();
	std::vector<std::string> members = def.getMembersOrder();
	for (auto it = members.end() - 1; it >= members.begin(); it--) {
		RPNValueType const type = def.getMemberType(*it);
		this->topVariable();
		if (this->hasErrors()) {
			return;
		}
		if (stack.top().type != type) {
			this->error = ExpressionResult("Struct member " + *it + " is of type " + type.name() +
											   " but got " + stack.top().name(),
										   stack.top().range, this->context);
			return;
		}
		range.merge(stack.top().range);
		stack.pop();
	}
	stack.emplace(name, range, false);
}

void Analyzer::analyzeKeyword(const KeywordToken *token) {
	switch (token->getKeyword()) {
		case KEYWORD_BREAK:
		case KEYWORD_CONTINUE:
			if (!this->stack.empty()) {
				this->error = ExpressionResult("Break and continue can't have any value",
											   token->getRange(), this->context);
				return;
			}
			break;
		case KEYWORD_RETURN:
			this->analyzeReturn(token);
			break;
		case KEYWORD_IMPORT:
			this->checkKeywordLine(token, false);
			if (this->hasErrors()) {
				return;
			}
			this->analyzeImport(token);
			break;
		case KEYWORD_IMPORTAS:
			this->checkKeywordLine(token, false);
			if (this->hasErrors()) {
				return;
			}
			this->analyzeImportAs(token);
			break;
		case KEYWORD_GET:
			this->checkKeywordLine(token, true, false);
			if (this->hasErrors()) {
				return;
			}
			this->analyzeGet(token);
			break;
		default:
			throw std::runtime_error("The keyword " + token->getStringValue() +
									 " is not implemented");
	}
}

void Analyzer::analyzeImport(const KeywordToken *token) {
	if (this->currentLine->totalSize() != 2) {
		this->error = ExpressionResult("import must have exactly one argument", token->getRange(),
									   this->context);
		return;
	}
	const std::string path = this->currentLine->last()->getStringValue();
	if (path.empty()) {
		this->error = ExpressionResult("import path cannot be empty",
									   this->currentLine->last()->getRange(), this->context);
		return;
	}
	this->error = Module::addModule(path, extractFileName(path),
									this->currentLine->last()->getRange(), context);
}

void Analyzer::analyzeImportAs(const KeywordToken *token) {
	if (this->currentLine->totalSize() != 3) {
		this->error = ExpressionResult("import must have exactly two arguments", token->getRange(),
									   this->context);
		return;
	}
	const std::string name = this->currentLine->last()->getStringValue();
	this->currentLine->goBack();
	const std::string path = this->currentLine->last()->getStringValue();
	this->currentLine->pop();
	if (path.empty()) {
		this->error = ExpressionResult("import path cannot be empty",
									   this->currentLine->last()->getRange(), this->context);
		return;
	}
	if (name.empty()) {
		this->error = ExpressionResult("import name cannot be empty",
									   this->currentLine->last()->getRange(), this->context);
		return;
	}
	this->error = Module::addModule(path, name, this->currentLine->last()->getRange(), context);
}

void Analyzer::analyzeReturn(const KeywordToken *token) {
	if (this->stack.size() > 1) {
		this->error = ExpressionResult("Return can't have more than one value", token->getRange(),
									   this->context);
		return;
	}
	if (this->stack.empty()) {
		this->error =
			ExpressionResult("Return must have a value", token->getRange(), this->context);
	}
	this->topVariable();
	if (this->hasErrors()) {
		return;
	}
	if (this->stack.top().type != this->currentFunctionReturnType) {
		this->error = ExpressionResult("Return type is " + this->currentFunctionReturnType.name() +
										   " but got " + this->stack.top().name(),
									   this->stack.top().range, this->context);
	}
}

void Analyzer::analyzePath(Token *path, bool addToStack) {
	bool isBuiltin = false;
	Path *p = dynamic_cast<Path *>(dynamic_cast<ValueToken *>(path)->getValue());
	this->error = Module::checkPath(p, this->context, isBuiltin);
	if (this->hasErrors()) {
		return;
	}
	p->setType(isBuiltin ? BUILTIN_PATH : PATH);
	if (!Module::hasValue(p)) {
		this->error =
			ExpressionResult("Module " + p->ats(0) + " doesn't define the value " + p->ats(1),
							 path->getRange(), this->context);
		return;
	}
	if (!addToStack) {
		return;
	}
	Value const *variable = Module::getModuleValue(p);
	if (variable->getType() != FUNCTION) {
		this->stack.emplace(variable->getType(), p->getRange(), false, 0, 0, false);
	} else if (!this->functions.contains(p->getStringValue())) {
	}
}

void Analyzer::checkKeywordLine(const KeywordToken *token, bool restaureStack, bool strict) {
	if (!linePatern.contains(token->getKeyword())) {
		this->checkRemainingCount();
	}
	std::vector<RPNValueType> types = linePatern.at(token->getKeyword());
	if ((strict && types.size() != this->stack.size()) || types.size() > this->stack.size()) {
		this->error =
			ExpressionResult("Keyword " + token->getStringValue() + " require " +
								 (strict ? "exactly " : "") + std::to_string(types.size()) +
								 " values but got " + std::to_string(this->stack.size()),
							 token->getRange(), this->context);
		return;
	}
	std::stack<AnalyzerValueType> values;
	for (int i = types.size() - 1; i >= 0; i--) {
		if (types[i].index() == 0) {
			if (stack.top().type.index() != 0) {
				this->error = ExpressionResult("Keyword " + token->getStringValue() +
												   " require a value of type " + types[i].name() +
												   " but got " + stack.top().name(),
											   stack.top().range, this->context);
				return;
			}
			if (std::get<std::string>(types[i].getType()) !=
				std::get<std::string>(stack.top().type.getType())) {
				this->error = ExpressionResult("Keyword " + token->getStringValue() +
												   " require a value of type " + types[i].name() +
												   " but got " + stack.top().name(),
											   stack.top().range, this->context);
				return;
			}
		} else {
			if (std::get<ValueType>(types[i].getType()) == VARIABLE) {
				if (stack.top().type.index() != 0) {
					this->error = ExpressionResult(
						"Keyword " + token->getStringValue() + " require a value of type " +
							types[i].name() + " but got " + stack.top().name(),
						stack.top().range, this->context);
					return;
				}
				if (!stack.top().isVariable) {
					this->error = ExpressionResult(
						"Keyword " + token->getStringValue() + " require a value of type " +
							types[i].name() + " but got " + stack.top().name(),
						stack.top().range, this->context);
					return;
				}
				this->variables[std::get<std::string>(stack.top().type.getType())] = {
					INT, stack.top().range, false, 0, 0, false};
				continue;
			}
			this->topVariable();
			if (this->hasErrors()) {
				return;
			}
			if (stack.top().type != types[i]) {
				this->error = ExpressionResult("Keyword " + token->getStringValue() +
												   " require a value of type " + types[i].name() +
												   " but got " + stack.top().name(),
											   stack.top().range, this->context);
				return;
			}
		}
		values.push(stack.top());
		stack.pop();
	}
	if (restaureStack) {
		while (!values.empty()) {
			stack.push(values.top());
			values.pop();
		}
	}
}

void Analyzer::analyzeStructAccess(const Token *token) {
	if (this->stack.empty()) {
		this->error =
			ExpressionResult("Struct access require a struct", token->getRange(), this->context);
		return;
	}
	std::vector<std::string> path =
		dynamic_cast<const Path *>(dynamic_cast<const ValueToken *>(token)->getValue())->getPath();
	std::string const variableName = this->stack.top().name();
	this->topVariable();
	if (this->hasErrors()) {
		return;
	}
	AnalyzerValueType structType = this->stack.top();
	this->stack.pop();
	if (structType.type.index() != 0) {
		this->error = ExpressionResult("Variable " + variableName + " is not a struct",
									   token->getRange(), this->context);
		return;
	}
	if (!Struct::structExists(std::get<std::string>(structType.type.getType()))) {
		this->error = ExpressionResult("Struct " + structType.name() + " does not exist",
									   token->getRange(), this->context);
		return;
	}
	StructDefinition const definition =
		Struct::getStructDefinition(std::get<std::string>(structType.type.getType()));
	RPNValueType type;
	for (size_t i = 0; i < path.size(); i++) {
		if (!definition.hasMember(path.at(i))) {
			this->error = ExpressionResult("Struct " + structType.name() +
											   " does not have a member named " + path.at(i),
										   token->getRange(), this->context);
			return;
		}
		type = definition.getMemberType(path.at(i));
		if (i < path.size() - 1 &&
			(type.index() != 0 || std::get<ValueType>(type.getType()) != STRUCT)) {
			this->error = ExpressionResult("Member " + path.at(i) + " of struct " +
											   structType.name() + " is not a struct",
										   token->getRange(), this->context);
			return;
		}
	}
	this->stack.emplace(type, token->getRange(), false, 0, 0, true, false);
}

void Analyzer::analyzeGet(const Token *token) {
	this->stack.pop();
	RPNValueType const type = this->stack.top().type;
	TextRange const range = this->stack.top().range;
	this->stack.pop();

	this->stack.emplace(type.getListType(), token->getRange().merge(range), false, 0, 0, false,
						true);
}

bool Analyzer::isBinaryOperator(OperatorToken::OperatorTypes operatorType) {
	return operatorType == OperatorToken::OP_ADD || operatorType == OperatorToken::OP_SUB ||
		   operatorType == OperatorToken::OP_MUL || operatorType == OperatorToken::OP_DIV ||
		   operatorType == OperatorToken::OP_POW || operatorType == OperatorToken::OP_MOD;
}

bool Analyzer::isComparisonOperator(OperatorToken::OperatorTypes operatorType) {
	return operatorType == OperatorToken::OP_EQ || operatorType == OperatorToken::OP_NE;
}

std::optional<ValueType> Analyzer::getOperatorType(ValueType left, ValueType right,
												   const OperatorToken *operatorToken) {
	OperatorToken::OperatorTypes const operatorType = operatorToken->getOperatorType();
	// for now, there is no way to différentiate return value of a function with ANY type as return
	// value
	if (left == ANY || right == ANY) {
		return ANY;
	}

	if (left == LIST) {
		if (operatorType == OperatorToken::OP_ADD && right == LIST) {
			return LIST;
		}
		if (operatorType == OperatorToken::OP_MUL && right == INT) {
			return LIST;
		} else if (Analyzer::isComparisonOperator(operatorType)) {
			return BOOL;
		}
		return std::nullopt;
	}
	if (right == LIST) {
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
		}
		if (operatorType == OperatorToken::OP_MUL && right == INT) {
			return STRING;
		} else if (Analyzer::isComparisonOperator(operatorType)) {
			return BOOL;
		}
		return std::nullopt;
	}
	if (right == STRING) {
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
		}
		if ((left == INT || left == BOOL) && right == FLOAT) {
			return FLOAT;
		} else if (left == FLOAT && (right == INT || right == BOOL)) {
			return FLOAT;
		} else if (left == FLOAT && right == FLOAT) {
			return FLOAT;
		}
	} else if (Analyzer::isComparisonOperator(operatorType)) {
		return BOOL;
	} else if (operatorType == OperatorToken::OP_GT || operatorType == OperatorToken::OP_GE ||
			   operatorType == OperatorToken::OP_LT || operatorType == OperatorToken::OP_LE) {
		if ((left == BOOL || left == INT || left == FLOAT) &&
			(right == BOOL || right == INT || right == FLOAT)) {
			return BOOL;
		}
	}

	return std::nullopt;
}

std::ostream &operator<<(std::ostream &stream, const AnalyzerValueType &valueType) {
	if (valueType.type.index() == 0) {
		stream << (valueType.isVariable
					   ? "variable "
					   : (valueType.isStructMember ? "struct member " : "struct "));
		stream << valueType.name();
	} else {
		stream << valueType.name();
	}
	return stream;
}