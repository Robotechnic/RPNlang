#include <csignal>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

#include "context/context.hpp"
#include "cppmodule/cppmodule.hpp"
#include "expressionresult/expressionresult.hpp"
#include "interpreter/interpreter.hpp"
#include "shell/colors.hpp"
#include "shell/shell.hpp"
#include "textutilities/textutilities.hpp"

/**
 * @brief allow shell to be destroyed when ctrl+c is pressed, this allow to save the history
 *
 * @param signum signal number
 */
void signalHandler(int signum) {
	if (signum == SIGINT) {
		return;
	}
	std::cout << "\033[0m" << std::endl;
	exit(signum);
}

bool multilineKeyword(std::string_view keyword) {
	return keyword == "if" || keyword == "while" || keyword == "for" || keyword == "fun" ||
		   keyword == "struct" || keyword == "try";
}

ExpressionResult getMultilineInput(std::deque<Token *> &tokens, const ContextPtr &ctx,
								   unsigned int &lineNumber) {
	std::string instruction{""};
	ExpressionResult result;
	if (!tokens.empty() && tokens.back()->getType() == TokenType::TOKEN_TYPE_LITERAL &&
		multilineKeyword(tokens.back()->getStringValue())) {
		rpnShell.setPrompt("... ");
		rpnShell >> instruction;
		lineNumber += instruction.size() != 0;
		int emptyLines = 0;
		while (!instruction.empty() || emptyLines < 2) {
			if (!instruction.empty()) {
				result = Lexer::tokenize(lineNumber, instruction, tokens, ctx);
				if (result.error()) {
					return result;
				}
				tokens.push_back(new StringToken(TextRange(lineNumber, instruction.size(), 1),
												 TokenType::TOKEN_TYPE_END_OF_LINE, "\n"));
			}
			rpnShell >> instruction;
			lineNumber += !instruction.empty();
			emptyLines += instruction.empty();
		}
		rpnShell.setPrompt(">>> ");
	}
	return result;
}

void displayInterpreterResult(const ExpressionResult &result, const Value *lastValue,
							  unsigned int lineNumber, std::string_view instruction, char lastChar) {
	if (result.error()) {
		if (result.getRange().line != lineNumber) {
			instruction = rpnShell.at(rpnShell.getHistorySize() - lineNumber);
		}
		result.displayLineError(instruction);
	} else if (lastValue == nullptr) {
		rpnShell << RED << "NONE" << DEFAULT << std::endl;
	} else {
		if (lastChar != '\n' && lastChar != '\r') {
			rpnShell << std::endl;
		}
		switch (lastValue->getType()) {
			case INT:
			case FLOAT:
				rpnShell << MAGENTA;
				break;
			case STRING:
				rpnShell << YELLOW;
				break;
			case BOOL:
				rpnShell << GREEN;
				break;
			case NONE:
				rpnShell << RED;
				break;
			default:
				break;
		}
		rpnShell << lastValue->getStringValue() << DEFAULT << std::endl;
	}
}

void shellInput() {
	// setup lastChar buffer
	std::streambuf *hold = std::cout.rdbuf();
	std::cout.flush();
	LastCharBuffer lastCharBuffer(hold);
	std::cout.rdbuf(&lastCharBuffer);

	const auto ctx = std::make_shared<Context>("main", "<stdin>");
	Interpreter i(ctx);
	std::string instruction;
	rpnShell.loadHistory();
	rpnShell >> instruction;
	std::deque<Token *> tokens;
	unsigned int lineNumber = 1;
	ExpressionResult result;
	while (instruction != "exit" && !std::cin.eof()) {
		result = Lexer::tokenize(lineNumber, instruction, tokens, ctx);
		if (result.error()) {
			result.displayLineError(instruction);
			continue;
		}
		result = getMultilineInput(tokens, ctx, lineNumber);
		if (result.error()) {
			result.displayLineError(instruction);
			continue;
		}
		Lexer lexer(tokens, ctx);
		result = lexer.lex();
		if (result.error()) {
			result.displayLineError(instruction);
		} else {
			Value::deleteValue(&i.getLastValue(), Value::INTERPRETER);
			result = i.interpret(lexer.getBlocks());
			ctx->copyTokenValues();
			displayInterpreterResult(result, i.getLastValue(), lineNumber, instruction,
									 lastCharBuffer.getLastChar());
			i.clearMemory();
		}
		tokens.clear();
		rpnShell >> instruction;
		if (instruction.empty()) {
			lineNumber++;
		}
	}
	// restaure hold buffer
	std::cout.rdbuf(hold);
}

int interpretPipe() {
	std::cout << "Interpret pipe" << std::endl;
	auto ctx = std::make_shared<Context>("main", "<stdin>");
	// display the stdin content
	std::string instruction;
	std::deque<Token *> tokens;
	unsigned int lineNumber = 0;
	std::vector<std::string> lines;
	ExpressionResult result;
	while (std::getline(std::cin, instruction)) {
		lineNumber++;
		lines.push_back(instruction);
		result = Lexer::tokenize(lineNumber, instruction, tokens, ctx);
		if (result.error()) {
			result.displayLineError(instruction);
			return 1;
		}
		tokens.push_back(new StringToken(TextRange(lineNumber, instruction.size(), 1),
										 TokenType::TOKEN_TYPE_END_OF_LINE, "\n"));
	}
	Lexer lexer(tokens, ctx);
	result = lexer.lex();
	if (result.error()) {
		result.displayLineError(lines[result.getRange().line - 1]);
		return 1;
	}
	Interpreter i(ctx);
	result = i.interpret(lexer.getBlocks());
	if (result.error()) {
		result.displayLineError(lines[result.getRange().line - 1]);
		return 1;
	}
	return 0;
}

void setWorkingDirectory(const std::string_view &path) {
	try {
		std::string extractedPath = extractFilePath(path);
		if (extractedPath[0] == '/') {
			std::filesystem::current_path(extractedPath);
		} else {
			std::filesystem::current_path(std::filesystem::current_path().string() + "/" +
										  extractedPath);
		}
	} catch (std::filesystem::filesystem_error &e) {
		std::cout << "Error: " << e.what() << std::endl;
		exit(1);
	}
}

int main(int argc, char **argv) {
	bool result = true;
#ifdef TEST_FILE
	std::string path;
	if (argc == 1) {
		path = TEST_FILE;
		argc = 2;
	} else {
		path = argv[1];
	}
#endif

	CppModule::setBuiltinModulesPath(
		std::filesystem::canonical(std::filesystem::current_path()).string() + "/RPNmodules");

	if (!isatty(fileno(stdin))) {
		return interpretPipe();
	}

	if (argc == 1) {
		signal(SIGTERM, signalHandler);
		signal(SIGINT, signalHandler);
		shellInput();
	} else {
#ifndef TEST_FILE
		std::string path = argv[1];
#endif
		setWorkingDirectory(path);
		std::string name = extractFileName(path);
		path = path.substr(path.find_last_of('/') + 1);
		ContextPtr ctx = std::make_shared<Context>(name, path, CONTEXT_TYPE_FILE);
		std::string error;
		result = Interpreter(ctx).interpretFile(path, error);
		if (!result) {
			std::cout << error << std::endl;
		}
	}

	std::cout << "\033[0m" << std::endl;
	return result ? 0 : 1;
}