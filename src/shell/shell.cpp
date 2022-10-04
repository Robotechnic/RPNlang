#include "shell/shell.hpp"

Shell::Shell(std::string historyFile) : 
	prompt(">>> "),
	historyIndex(0),
	cursorPosition(0),
	command("") {
	if (historyFile != "") {
		this->historyFile = historyFile;
	} else {
		this->historyFile = std::getenv("HOME");
		this->historyFile += "/.rpn_history";
	}
	this->loadHistory();
}

Shell::~Shell() {
	this->saveHistory();
}

/**
 * @brief get a command from the user with history and cursor management
 * 
 * @return std::string the command typed by the user
 */
std::string Shell::getCommand() {
	this->command = "";
	this->historyIndex = 0;
	this->cursorPosition = 0;
	std::cout<<this->prompt;
	char c;
	while ((c = this->getChar()) != '\n') {
		if (this->isSpecialChar(c)) {
			this->handleSpecialChar(c);
		} else {
			this->putChar(c);
		}
	}
	std::cout<<std::endl;
	if (command != "" && (this->history.size() == 0 || this->history.back() != command)) {
		this->history.push_back(command);
		if (this->history.size() > MAX_HISTORY_SIZE) {
			this->history.erase(this->history.begin());
		}
	}
	return this->command;
}

/**
 * @brief Load the command history from the history file
 * 
 */
void Shell::loadHistory() {
	std::ifstream file;
	try {
		file.open(this->historyFile);
		if (file.fail()) {
			std::cout<<"Failled to open history file at '" << this->historyFile << "' (" << std::strerror(errno) << ")"<<std::endl;
			return;
		}
	} catch (const std::exception &e) {
		std::cout << "History stream error :" << std::string(e.what()) << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		this->history.push_back(line);
	}

	file.close();
}

/**
 * @brief save the command history to the history file
 * 
 */
void Shell::saveHistory() {
	std::ofstream file;
	try {
		file.open(this->historyFile, std::ios::trunc);
		if (file.fail()) {
			std::cout<<"Failled to save history at '" << this->historyFile << "' (" << std::strerror(errno) << ")"<<std::endl;
			exit(1);
		}
	} catch (const std::exception &e) {
		std::cout << "History stream error :" << std::string(e.what()) << std::endl;
		exit(1);
	}

	std::cout<<"Saving history to '" << this->historyFile << "'"<<std::endl;
	std::cout<<"History size : " << this->history.size() << std::endl;

	for (std::string line : this->history) {
		file << line << std::endl;
	}

	file.close();
}

/**
 * @brief check if the given character is a special character
 * 
 * @param c the character to check
 * @return bool true if the character is a special character
 */
bool Shell::isSpecialChar(char c) {
	return c == 27 || c == 127 || c == 8 || c == 4;
}

/**
 * @brief go to line beginning and rewrite the line
 * 
 */
void Shell::updateLine() {
	std::cout<<"\033[0K";
	for (size_t i = this->cursorPosition; i < this->command.size(); i++)
		std::cout<<command[i];
	this->setCursorPosition(this->cursorPosition + 1);
}

/**
 * @brief treat a given special character to handle his action
 * 
 * @param c the special character to treat
 */
void Shell::handleSpecialChar(char c) {
	if (c == 127) {
		this->popChar();
	} else if (c == 8) {
		this->popWord();
	} else if (c == 27) { // arrow keys, home/end and supr
		this->arrowManagement();
	} else if (c == 4) {
		std::cout<<std::endl;
		this->saveHistory();
		exit(0);
	}
}

/**
 * @brief moove the cursor according to the pressed keys
 * 
 */
void Shell::arrowManagement() {
	char bracket = this->getChar();
	if (bracket != 91) { // '[' character
		this->putChar(bracket);
		return;
	}
	char type = this->getChar();
	switch (type) {
		case 68: //left
			this->arrowLeft();
			break;
		case 67: //right
			this->arrowRight();
			break;
		case 65: //up
			this->arrowUp();
			break;
		case 66: //down
			this->arrowDown();
			break;
		case 72: //home
			this->cursorPosition = 0;
			this->setCursorPosition(0);
			break;
		case 70: //end
			this->cursorPosition = this->command.size();
			this->setCursorPosition(this->command.size());
			break;
		case 51: //supr
			type = this->getChar();
			if (type == 126) {
				this->popCharRight();
			} else {
				// skip the next two char which mean the ctrl key
				for (int i = 0; i < 2; i++) {
					this->getChar();
				}
				this->popWordRight();
			}
			break;
		case 49: // ctrl + arrow
			for (int i = 0; i < 2; i++) {
				this->getChar();
			}
			type = this->getChar();
			if (type == 68) {
				this->arrowLeft(true);
			} else if (type == 67) {
				this->arrowRight(true);
			}
			break;
		default:
			return;
	}
}

/**
 * @brief moove the cursor to the right
 * 
 * @param ctrl if the ctrl key is pressed
 */
void Shell::arrowRight(bool ctrl) {
	if (this->cursorPosition >= (int)this->command.size()) return;
	
	do {
		this->cursorPosition++;
		this->mooveRight();
	} while (
		ctrl &&
		this->command[this->cursorPosition] != ' ' && 
		this->cursorPosition < (int)this->command.size()
	);
}

/**
 * @brief moove the cursor to the left
 * 
 * @param ctrl if the ctrl key is pressed
 */
void Shell::arrowLeft(bool ctrl) {
	if (this->cursorPosition <= 0) return;

	do {
		this->cursorPosition--;
		this->mooveLeft();
	} while (
		ctrl &&
		this->command[this->cursorPosition] != ' ' && 
		this->cursorPosition > 0
	);
}

/**
 * @brief go to the previous history command
 * 
 */
void Shell::arrowUp() {
	if (this->historyIndex >= (int)this->history.size()) return;
	if (this->historyIndex <= 0) {
		this->savedCommand = this->command;
	}
	this->historyIndex++;
	this->displayHistoryLine();
}

/**
 * @brief got the the next history command
 * 
 */
void Shell::arrowDown() {
	if (this->historyIndex > 0) {
		this->historyIndex--;
		this->displayHistoryLine();
	} else {
		this->command = this->savedCommand;
		this->setLine();
	}
}

/**
 * @brief get a char fom the user without echo
 * 
 * @return char the char readen
 */
char Shell::getChar() {
	char c;
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~ICANON;
	newt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	c = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return c;
}

/**
 * @brief moove the console cursor to the left
 * 
 */
void Shell::mooveLeft() {
	std::cout<<"\033[1D";
}

/**
 * @brief moove the console cursor to the right
 * 
 */
void Shell::mooveRight() {
	std::cout<<"\033[1C";
}

/**
 * @brief add a char to the command and display it correctly in the console
 * 
 * @param c the char to add
 */
void Shell::putChar(char c) {
	this->command.insert(this->cursorPosition, std::string(1, c));
	for (size_t i = this->cursorPosition; i < this->command.size(); i++)
		std::cout<<command[i];
	this->cursorPosition++;
	for (int i = this->command.size(); i > this->cursorPosition; i--)
		this->mooveLeft();
}

/**
 * @brief delete the char at the cursor position
 * 
 */
void Shell::popChar() {
	if (this->cursorPosition <= 0) return;
	// delete char
	this->command.erase(this->cursorPosition - 1, 1);
	this->mooveLeft();
	this->updateLine();
}

/**
 * @brief delete the next left word at the cursor position
 * 
 */
void Shell::popWord() {
	if (this->cursorPosition <= 0) return;
	// delete a word from the cursor position
	do {
		this->command.erase(this->cursorPosition, 1);
		this->mooveLeft();
		this->cursorPosition--;
	} while (this->cursorPosition >= 0 && this->command[this->cursorPosition] != ' ');
	
	if (this->cursorPosition < 0) {
		this->cursorPosition = 0;
	}

	this->updateLine();
}

/**
 * @brief delete the next right char at the cursor position
 * 
 */
void Shell::popCharRight() {
	if (this->cursorPosition >= (int)this->command.size()) return;
	// delete char
	this->command.erase(this->cursorPosition, 1);
	
	if (this->cursorPosition >= (int)this->command.size()) {
		this->cursorPosition = this->command.size();
	}

	this->updateLine();
}

/**
 * @brief delete the next right word at the cursor position
 * 
 */
void Shell::popWordRight() {
	if (this->cursorPosition >= (int)this->command.size()) return;
	// delete a word from the cursor position
	do {
		this->command.erase(this->cursorPosition, 1);
	} while (this->cursorPosition < (int)this->command.size() && this->command[this->cursorPosition] != ' ');

	this->updateLine();
}

/**
 * @brief errase the current console line
 * 
 */
void Shell::eraseLine() {
	std::cout<<"\033[2K";
	std::cout<<"\r";
	std::cout<<this->prompt;
}

/**
 * @brief set the console cursor position
 * 
 * @param position the position to set
 */
void Shell::setCursorPosition(int position) {
	position += this->prompt.size();
	std::cout<<"\033[" << position << "G";
}

/**
 * @brief display the current history line in the console and reset the cursor position to the end of the line
 * 
 */
void Shell::displayHistoryLine() {
	this->command = this->history[this->history.size() - this->historyIndex];
	this->setLine();
}

/**
 * @brief display the current command in the console and reset the cursor position to the end of the line
 * 
 */
void Shell::setLine() {
	this->eraseLine();
	this->cursorPosition = this->command.size();
	std::cout<<this->command;
}

void Shell::operator>>(std::string& str) {
	str = this->getCommand();
}

Shell& operator<<(Shell& out, std::ostream& (*os)(std::ostream&)) {
	os(std::cout);
	return out;
}

Shell& operator<<(Shell& out, const std::string &str) {
	std::cout<<str;
	return out;
}

Shell& operator<<(Shell& out,const color &c) {
	std::cout<<toEscapeSequence(c);
	return out;
}

Shell& operator<<(Shell& out, const background &b) {
	std::cout<<toEscapeSequence(b);
	return out;
}

Shell& operator<<(Shell& out, const format &s) {
	std::cout<<toEscapeSequence(s);
	return out;
}