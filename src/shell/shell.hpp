#pragma once
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <termios.h>
#include <unistd.h>
#include <vector>

#include "shell/colors.hpp"

#define MAX_HISTORY_SIZE 200

class Shell {
  public:
	Shell();
	~Shell();
	std::string getCommand();

	bool loadHistory(std::string_view historyFile = "");
	void saveHistory();
	void setPrompt(std::string_view prompt);

	size_t getHistorySize() const;
	std::string_view at(size_t index) const;

	void operator>>(std::string &str);

  private:
	static char getChar();
	void skipChar(int n = 1);

	static bool isSpecialChar(char c);

	void updateLine(int offset = 1);

	void handleSpecialChar(char c);
	void arrowManagement();

	void arrowRight(bool ctrl = false);
	void arrowLeft(bool ctrl = false);
	void arrowUp();
	void arrowDown();

	static void mooveLeft();
	static void mooveRight();

	void setCursorPosition(int position);
	void eraseLine();

	void putChar(char c);
	void popChar();
	void popWord();

	void popCharRight();
	void popWordRight();

	void displayHistoryLine();
	void setLine();

	std::string prompt;

	int historyIndex;
	int cursorPosition;
	std::string command;
	std::string savedCommand;
	std::string historyFile;
	std::vector<std::string> history;
};

extern Shell rpnShell;

Shell &operator<<(Shell &out, std::ostream &(*os)(std::ostream &));
Shell &operator<<(Shell &out, std::string_view str);
Shell &operator<<(Shell &out, const color &c);
Shell &operator<<(Shell &out, const background &b);
Shell &operator<<(Shell &out, const format &s);

class LastCharBuffer : public std::streambuf {
  public:
	explicit LastCharBuffer(std::streambuf *sb) : buff(sb) {
		setp(0, 0);
	};
	char getLastChar() const {
		return lastChar;
	};

	int_type overflow(int_type currentChar) override {
		lastChar = currentChar;
		return buff->sputc(currentChar);
	};

  private:
	std::streambuf *buff;
	char lastChar = traits_type::eof();
};