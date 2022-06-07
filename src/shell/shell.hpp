#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <termios.h>
#include <unistd.h>

#include "shell/colors.hpp"

#define MAX_HISTORY_SIZE 100

class Shell {
	public:
		Shell(std::string historyFile = "");
		~Shell();
		std::string getCommand();

		void loadHistory();
		void saveHistory();

		void operator>>(std::string& str);

	private:
		char getChar();
		bool isSpecialChar(char c);

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

		const std::string prompt;

		int historyIndex, cursorPosition;
		std::string command, savedCommand;
		std::string historyFile;
		std::vector<std::string> history;
};


Shell& operator<<(Shell& out, std::ostream& (*os)(std::ostream&));
Shell& operator<<(Shell& out, const std::string &str);
Shell& operator<<(Shell& out,const color &c);
Shell& operator<<(Shell& out, const background &b);
Shell& operator<<(Shell& out, const format &s);