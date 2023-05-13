#include "interpreter/interpreter.hpp"

Interpreter::Interpreter()
	: lastValue(nullptr), context(std::make_shared<Context>("main", "<stdin>")) {}

Interpreter::Interpreter(ContextPtr ctx) : lastValue(nullptr), context(ctx) {}

Interpreter::~Interpreter() {
	this->clearMemory();
}

void Interpreter::clearMemory() {
	Value::deleteValue(&this->lastValue, Value::INTERPRETER);
	this->memory.clear();
}

bool Interpreter::interpretFile(std::string_view fileName, std::string &errorString,
								bool isModule) {
	std::ifstream file;
	if (!openFile(file, fileName, errorString)) {
		return false;
	}

	bool error = false;
	std::deque<Token *> tokens;
	std::string instruction;
	int line = 0;
	ExpressionResult result;
	while (!error && std::getline(file, instruction)) {
		line++;
		result = Lexer::tokenize(line, instruction, tokens, this->context);
		if (result.error()) {
			result.display();
			error = true;
		}
		tokens.push_back(new StringToken(line, instruction.size(), TokenType::TOKEN_TYPE_END_OF_LINE, "\n"));
	}
	file.close();

	if (error) {
		return false;
	}
	Lexer lexer(tokens, this->context);
	result = lexer.lex();
	if (result.error()) {
		result.display();
		return false;
	}

	Value::deleteValue(&this->lastValue, Value::INTERPRETER);
	result = this->interpret(lexer.getBlocks());
	if (result.error()) {
		result.display();
		return false;
	}
	if (this->context->hasValue("main") && this->context->getType() != CONTEXT_TYPE_MODULE &&
		this->context->getType() != CONTEXT_TYPE_BUILTIN_MODULE) {
		Function *val = dynamic_cast<Function *>(this->context->getValue("main"));
		TextRange const mainRange = val->getRange();
		if (val->getType() == FUNCTION) {
			const RPNFunction *func = val->getValue();
			RPNFunctionArgsValue args;
			RPNFunctionResult mainResult = func->call(args, mainRange, this->context);
			if (auto *callExpressionResult = std::get_if<ExpressionResult>(&mainResult);
				(callExpressionResult != nullptr) && callExpressionResult->error()) {
				callExpressionResult->display();
				return false;
			}
			Value::deleteValue(&std::get<Value *>(mainResult), Value::INTERPRETER);
		}
	}

	if (isModule) {
		this->context->takeOwnership();
	} else {
		this->context->clear();
	}

	return true;
}

/**
 * @brief return the last value remaining in the interpreter memory
 *
 * @return Value the last value in the stack
 */
Value *Interpreter::getLastValue() const {
	return this->lastValue;
}

Value *&Interpreter::getLastValue() {
	return this->lastValue;
}

/**
 * @brief check if memory contains less than 1 value at the end of the interpretation
 *
 * @return ExpressionResult error if memory contains more than 1 value
 */
void Interpreter::checkMemory() {
	if (this->memory.empty()) {
		Value::deleteValue(&this->lastValue, Value::INTERPRETER);
		this->lastValue = None::empty();
		return;
	}

	Value::deleteValue(&this->lastValue, Value::INTERPRETER);
	this->lastValue = this->memory.popVariableValue(this->context);
}

/**
 * @brief interpret a list of blocks
 *
 * @param blocks the list of blocks to interpret
 * @param free if true, free the blocks after interpretation
 * @return ExpressionResult status of the interpretation
 */
ExpressionResult Interpreter::interpret(BlockQueue &blocks) {
	ExpressionResult result;
	BlockQueueIterator it = blocks.begin();
	while (it && !result.stopInterpret()) {
		BaseBlock *block = *(it++);
		if (block->getType() == LINE_BLOCK) {
			Line *l = dynamic_cast<Line *>(block);
			if (it && (*it)->getType() == CODE_BLOCK) {
				result = this->interpretBlock(*l, *dynamic_cast<CodeBlock *>(*(it++)));
			} else {
				result = interpretLine(*l);
			}
		} else if (block->getType() == FUNCTION_BLOCK) {
			FunctionBlock const *f = dynamic_cast<FunctionBlock *>(block);
			this->context->setValue(f->getName(), new Function(f->getFunction(), f->lastRange(),
															   Value::CONTEXT_VARIABLE));
		} else {
			result = ExpressionResult("Lexer didn't to its job corectly ;(",
									  dynamic_cast<CodeBlock *>(block)->getRange(), this->context);
		}
	}
	return result;
}

/**
 * @brief interpret a line of tokens
 *
 * @param line the line to interpret
 * @return ExpressionResult status of the interpretation
 */
ExpressionResult Interpreter::interpretLine(Line &line, bool clearMemory) {
	ExpressionResult result;
	LineIterator it = line.begin();
	while (!result.stopInterpret() && it) {
		switch ((*it)->getType()) {
			case TokenType::TOKEN_TYPE_VALUE:
			case TokenType::TOKEN_TYPE_LITERAL:
			case TokenType::TOKEN_TYPE_PATH:
			case TokenType::TOKEN_TYPE_STRUCT_ACCESS:
				this->memory.push(dynamic_cast<ValueToken *>(*it)->getValue());
				break;
			case TokenType::TOKEN_TYPE_OPERATOR:
			case TokenType::TOKEN_TYPE_BOOLEAN_OPERATOR:
				result = this->interpretOperator(dynamic_cast<OperatorToken *>(*it));
				break;
			case TokenType::TOKEN_TYPE_FUNCTION_CALL:
			case TokenType::TOKEN_TYPE_MODULE_FUNCTION_CALL:
				result = this->interpretFunctionCall(*it);
				break;
			case TokenType::TOKEN_TYPE_FSTRING:
				this->interpretFString(dynamic_cast<FStringToken *>(*it));
				break;
			case TokenType::TOKEN_TYPE_ASSIGNMENT:
				this->interpretAssignment(*it);
				break;
			case TokenType::TOKEN_TYPE_KEYWORD:
				result = this->interpretKeyword(*it);
				break;
			case TokenType::TOKEN_TYPE_VALUE_TYPE:
				result = this->interpretValueType(*it);
				break;
			case TokenType::TOKEN_TYPE_STRUCT_NAME:
				result = this->interpretStruct(*it);
				break;
			default:
				result =
					ExpressionResult("Unknow token (TODO) : " + Token::stringType((*it)->getType()),
									 (*it)->getRange(), this->context);
				break;
		}
		it++;
	}
	if (result.error()) {
		return result;
	}
	if (clearMemory) {
		this->checkMemory();
	}
	return result;
}

/**
 * @brief interpret a block of code
 *
 * @param line the line which contains the block parameters
 * @param block the block to interpret
 * @return ExpressionResult status of the interpretation
 */
ExpressionResult Interpreter::interpretBlock(Line &line, CodeBlock &block) {
	switch (block.getKeyword()) {
		case KEYWORD_IF:
			return this->interpretIf(line, block);
		case KEYWORD_WHILE:
			return this->interpretWhile(line, block);
		case KEYWORD_FOR:
			return this->interpretFor(line, block);
		case KEYWORD_TRY:
			return this->interpretTry(line, block);
		default:
			return {"Unknow block type", block.getRange(), this->context};
	}
	return {};
}

void Interpreter::interpretFString(const FStringToken *token) {
	TextRange range = token->getRange();
	std::string str;
	Value *value = nullptr;
	for (size_t i = token->size() - 1; i > 0; i--) {
		value = this->memory.popVariableValue(this->context);
		str = value->getStringValue() + token->at(i) + str;
		if (i == 1) {
			range.merge(value->getRange());
		}
		Value::deleteValue(&value, Value::INTERPRETER);
	}
	str = token->getParts().at(0) + str;
	this->memory.push(new String(str, range, Value::INTERPRETER));
}

ExpressionResult Interpreter::interpretOperator(const OperatorToken *operatorToken) {
	Value *right = nullptr;
	Value *left = nullptr;
	right = this->memory.popVariableValue(this->context);

	// check for 0 division error
	if (operatorToken->getOperatorType() == OperatorToken::OP_DIV ||
		operatorToken->getOperatorType() == OperatorToken::OP_MOD) {
		if ((right->getType() == INT && dynamic_cast<Int *>(right)->getValue() == 0) ||
			(right->getType() == FLOAT && dynamic_cast<Float *>(right)->getValue() == 0) ||
			(right->getType() == BOOL &&
			 static_cast<int>(dynamic_cast<Bool *>(right)->getValue()) == 0)) {
			return {"Division by 0", right->getRange(), this->context};
		}
	}
	left = this->memory.popVariableValue(this->context);

	if ((left->getType() == STRING || left->getType() == LIST)) {
		Int const *number = dynamic_cast<Int *>(right);
		if (number && number->getValue() < 0) {
			return {"Cannot multiply list like object by a negative number", right->getRange(),
					this->context};
		}
	}

	this->memory.push(left->applyOperator(right, operatorToken, this->context));
	Value::deleteValue(&right, Value::INTERPRETER);
	if (right != left) {
		Value::deleteValue(&left, Value::INTERPRETER);
	}

	return {};
}

ExpressionResult Interpreter::interpretKeyword(const Token *keywordToken) {
	switch (dynamic_cast<const KeywordToken *>(keywordToken)->getKeyword()) {
		case KEYWORD_BREAK:
			return {ExpressionResult::BREAK};
		case KEYWORD_CONTINUE:
			return {ExpressionResult::CONTINUE};
		case KEYWORD_RETURN:
			return {ExpressionResult::RETURN};
		case KEYWORD_IMPORT:
		case KEYWORD_IMPORTAS: // this case is handle by analyser
			return {};
		case KEYWORD_GET:
			return this->interpretGet(keywordToken);
		default:
			return {"Unknow keyword " + keywordToken->getStringValue(), keywordToken->getRange(),
					this->context};
	}
}

ExpressionResult Interpreter::interpretValueType(const Token *typeToken) {
	ValueType const type =
		std::get<ValueType>(dynamic_cast<const TypeToken *>(typeToken)->getValueType().type);
	if (type == LIST && !RPNValueType::isCastableTo(this->memory.top()->getType(), LIST)) {
		return this->interpretList(typeToken);
	}
	Value *top = this->memory.popVariableValue(this->context);
	Value *value = top->to(type, Value::INTERPRETER);
	Value::deleteValue(&top, Value::INTERPRETER);
	this->memory.push(value);
	return {};
}

void Interpreter::interpretAssignment(const Token * /*operatorToken*/) {
	bool const copy = this->memory.top()->getType() == VARIABLE ||
					  this->memory.top()->getType() == PATH ||
					  this->memory.top()->getOwner() == Value::OBJECT_VALUE;

	Value *left = this->memory.popVariableValue(this->context);
	Value *hold = nullptr;
	if (this->memory.top()->getType() == VARIABLE) {
		this->context->setValue(this->memory.top(), copy ? left->copy() : left, &hold);
	} else if (this->memory.top()->getType() == STRUCT_ACCESS) {
		Path *path = dynamic_cast<Path *>(this->memory.top());
		this->memory.pop();
		Value *name = this->memory.pop();
		Value *structValue = Struct::getStruct(name, path, this->context);
		dynamic_cast<Struct *>(structValue)->setMember(path, copy ? left->copy() : left, &hold);
		this->memory.push(name);
		this->memory.push(path);
	} else {
		ListElement *element = dynamic_cast<ListElement *>(this->memory.top());
		element->set(copy ? left->copy() : left, &hold);
	}
	if (this->lastValue == hold) {
		this->lastValue = nullptr;
	}
}

const RPNFunction *Interpreter::getFunction(const Value *functionName) {
	if (functionName->getType() != PATH &&
		builtins::builtinFunctions.contains(functionName->getStringValue())) {
		return &builtins::builtinFunctions.at(functionName->getStringValue());
	}
	return dynamic_cast<Function *>(functionName->getType() == VARIABLE
										? this->context->getValue(functionName)
										: Module::getModuleValue(functionName))
		->getValue();
}

ExpressionResult Interpreter::interpretFunctionCall(Token *functionToken) {
	const RPNFunction *function = nullptr;
	Value const *functionName = dynamic_cast<ValueToken *>(functionToken)->getValue();
	function = this->getFunction(functionName);

	std::vector<Value *> arguments;
	for (size_t i = 0; i < function->getArgumentsCount(); i++) {
		arguments.insert(arguments.begin(), this->memory.popVariableValue(this->context));
	}

	RPNFunctionResult callResult;
	if (functionToken->getType() == TokenType::TOKEN_TYPE_FUNCTION_CALL) {
		callResult = function->call(arguments, functionName->getRange(), this->context);
	} else {
		ContextPtr const ctx = Module::getModuleContext(functionName, this->context);
		callResult = function->call(arguments, functionName->getRange(), ctx);
	}
	if (auto const *callExpressionResult = std::get_if<ExpressionResult>(&callResult);
		(callExpressionResult != nullptr) && callExpressionResult->error()) {
		return *callExpressionResult;
	}
	Value *callReturnValue = std::get<Value *>(callResult);
	if (!arguments.empty()) {
		callReturnValue->setVariableRange(
			TextRange::merge(functionName->getRange(), arguments.front()->getRange()));
	} else {
		callReturnValue->setVariableRange(functionName->getRange());
	}
	for (Value *value : arguments) {
		Value::deleteValue(&value, Value::INTERPRETER);
	}

	this->memory.push(callReturnValue);
	return {};
}

ExpressionResult Interpreter::interpretIf(Line &line, CodeBlock &block) {
	if (ExpressionResult result = this->interpretLine(line); result.error()) {
		return result;
	}
	Value *condition = this->lastValue->to(BOOL);
	if (dynamic_cast<Bool *>(condition)->getValue()) {
		Value::deleteValue(&condition, Value::INTERPRETER);
		return this->interpret(block.getBlocks());
	}
	Value::deleteValue(&condition, Value::INTERPRETER);
	if (block.getNext() != nullptr) {
		return this->interpret(block.getNext()->getBlocks());
	}

	return {};
}

ExpressionResult Interpreter::interpretWhile(Line &line, CodeBlock &block) {
	ExpressionResult result = this->interpretLine(line);
	if (result.error()) {
		return result;
	}
	Value *condition = this->lastValue->to(BOOL);
	while (dynamic_cast<Bool *>(condition)->getValue()) {
		Value::deleteValue(&condition, Value::INTERPRETER);
		block.reset();
		result = this->interpret(block.getBlocks());
		if (result.error()) {
			return result;
		}
		if (result.breakingLoop()) {
			return {};
		}
		if (result.returnValue()) {
			return result;
		}
		result = this->interpretLine(line);
		if (result.error()) {
			return result;
		}
		condition = this->lastValue->to(BOOL);
	}
	Value::deleteValue(&condition, Value::INTERPRETER);
	return {};
}

ExpressionResult Interpreter::interpretFor(Line &line, CodeBlock &block) {
	ExpressionResult result = this->interpretLine(line, false);
	if (result.error()) {
		return result;
	}

	std::vector<Int *> forParams;
	Value *param = nullptr;
	for (int i = 0; i < 3; i++) {
		param = this->memory.popVariableValue(this->context);
		forParams.emplace(forParams.begin(), dynamic_cast<Int *>(param));
	}

	Value *variable = this->memory.pop();
	Int zero{0, TextRange(), Value::PARENT_FUNCTION};
	CPPInterface const step{forParams.at(2)};
	if (step == &zero) {
		Value::deleteValue(&variable, Value::INTERPRETER);
		return {"Step can't be 0", forParams.at(2)->getRange(), this->context};
	}

	CPPInterface i{forParams.at(0)};
	while (!result.breakingLoop() && !result.returnValue() &&
		   ((step > &zero && i < forParams.at(1)) || (step < &zero && i > forParams.at(1)))) {
		this->context->setValue(variable->getStringValue(), i.getValue());
		result = this->interpret(block.getBlocks());
		if (result.error()) {
			return result;
		}
		if (!result.breakingLoop()) {
			i += step;
		}
	}
	if (i.getValue() != forParams.at(0)) {
		Value::deleteValue(&i.getValue(), Value::INTERPRETER);
	}
	Value::deleteValue(&variable, Value::INTERPRETER);
	for (Value *param : forParams) {
		Value::deleteValue(&param, Value::INTERPRETER);
	}
	if (result.returnValue()) {
		return result;
	}
	return {};
}

ExpressionResult Interpreter::interpretTry(Line &line, CodeBlock &block) {
	// try
	ExpressionResult result = this->interpret(block.getBlocks());
	if (block.getNext() == nullptr) {
		return result;
	}

	// finally
	if (!result.error()) {
		if (block.getNext()->getKeyword() == KEYWORD_FINALLY) {
			return this->interpret(block.getNext()->getBlocks());
		}
		return result;
	}

	// catch
	if (block.getNext()->getKeyword() != KEYWORD_CATCH) {
		if (block.getNext()->getKeyword() == KEYWORD_FINALLY) {
			return this->interpret(block.getNext()->getBlocks());
		}
		return {};
	}

	this->context->setValue(
		line.top()->getStringValue(),
		new String(result.getErrorMessage(), line.top()->getRange(), Value::CONTEXT_VARIABLE));

	result = this->interpret(block.getNext()->getBlocks());
	if (result.error()) {
		return result;
	}

	// finally
	if (block.getNext()->getNext() != nullptr) {
		return this->interpret(block.getNext()->getNext()->getBlocks());
	}

	return result;
}

ExpressionResult Interpreter::interpretList(const Token *keywordToken) {
	Int const *size = dynamic_cast<Int *>(this->memory.pop());
	if (size->getValue() < 0) {
		return {"List size must be positive", size->getRange(), this->context};
	}
	TextRange range = keywordToken->getRange().merge(size->getRange());
	std::vector<Value *> values;
	Value *value = nullptr;
	for (int i = 0; i < size->getValue(); i++) {
		value = this->memory.popVariableValue(this->context);
		if (value->getOwner() == Value::OBJECT_VALUE) {
			value = value->copy();
		}
		value->setOwner(Value::OBJECT_VALUE);
		values.emplace(values.begin(), value);
		range.merge(values.at(0)->getRange());
	}
	this->memory.push(new List(values, range, Value::INTERPRETER,
							   dynamic_cast<const TypeToken *>(keywordToken)->getListType()));
	return {};
}

ExpressionResult Interpreter::interpretStruct(const Token *keywordToken) {
	std::string const name = keywordToken->getStringValue();
	if (!Struct::structExists(name)) {
		return {"Undefined struct name : " + std::string(name), keywordToken->getRange(),
				this->context};
	}
	size_t const count = Struct::getStructMembersCount(name);
	std::vector<Value *> members(count, nullptr);
	TextRange range = keywordToken->getRange();
	for (size_t i = count; i > 0U; i--) {
		members.at(i - 1) = this->memory.popVariableValue(this->context);
		if (members.at(i - 1)->getOwner() == Value::OBJECT_VALUE) {
			members.at(i - 1) = members.at(i - 1)->copy();
		}
		members.at(i - 1)->setOwner(Value::OBJECT_VALUE);
	}
	range.merge(members.at(0)->getRange());
	Struct *s = new Struct(range, name, Value::INTERPRETER);
	s->setMembers(members, this->context);
	this->memory.push(s);
	return {};
}

ExpressionResult Interpreter::interpretGet(const Token *keywordToken) {
	Value *index = this->memory.popVariableValue(this->context);
	int64_t const i = dynamic_cast<Int *>(index)->getValue();
	List *list = dynamic_cast<List *>(this->memory.popVariableValue(this->context));
	if (i < 0 || i >= list->size()) {
		return {"Index out of range", index->getRange(), this->context};
	}
	this->memory.push(new ListElement(list, i, keywordToken->getRange().merge(index->getRange()),
									  Value::INTERPRETER));
	Value::deleteValue(&index, Value::INTERPRETER);
	return {};
}
