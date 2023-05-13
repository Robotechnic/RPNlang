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
	char getChar();
	void skipChar(int n = 1);

	bool isSpecialChar(char c);

	void updateLine(int offset = 1);

	void handleSpecialChar(char c);
	void arrowManagement();

	void arrowRight(bool ctrl = false);
	void arrowLeft(bool ctrl = false);
	void arrowUp();
	void arrowDown();

	void mooveLeft();
	void mooveRight();

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

	int historyIndex, cursorPosition;
	std::string command, savedCommand;
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
	LastCharBuffer(std::streambuf *sb) : buff(sb), lastChar(traits_type::eof()) {
		setp(0, 0);
	};
	char getLastChar() const {
		return lastChar;
	};

	virtual int_type overflow(int_type c) {
		lastChar = c;
		return buff->sputc(c);
	};

  private:
	std::streambuf *buff;
	char lastChar;
};