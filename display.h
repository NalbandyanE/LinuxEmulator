#ifndef LINUX_EMULATOR_DISPLAY_H
#define LINUX_EMULATOR_DISPLAY_H

#include "database.h"
#include "commandexecutor.h"
#include "user.h"

#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <memory>
#include <cstdio>
#include <stdexcept>
#include <sstream>

namespace LinuxEmulator {

class Display {
public:
	Display();
	void run();
    void runTerminal();
    void runExam();
    void runVirtualTerminal(const std::string&, const std::string&);
private:
	Database db;
	FileSystem fsMy;
    FileSystem fsUser;
    CommandExecutor ceMy;
    CommandExecutor ceUser;
};

Display::Display() : fsMy(), fsUser(), ceMy(fsMy), ceUser(fsUser) {}

void Display::run() {
    std::cout << "Hello, this is a Linux Emulator created by Elmira Nalbandyan.\nYou can pass Linux Badge Exam with me.\n"
                 "Also you can use me as terminal only.\n";
    int mode;
    std::cout << "Select mode:\n";
    std::cout << "1. Termianl Mode\n";
    std::cout << "2. Exam Mode\n";
    std::cout << "Enter your choice: ";
    std::cin >> mode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    switch (mode) {
        case 1:
            runTerminal();
            break;
        case 2:
            runExam();
            break;
        default:
            std::cout << "Invalid mode selection. Exiting...\n";
            break;
    }
}

void printColoredText(const std::string&, int);

void Display::runTerminal() {
    FileSystem fs;
    std::cout << "Welcome! Let's begin." << std::endl;
    std::string username;
    std::cout << "Input username: ";
    std::getline(std::cin, username);
    std::string password;
    std::cout << "Input password: ";
    std::getline(std::cin, password);
    User user(username, password);
    CommandExecutor ce(fs, user);
    std::string answer;
    while (true) {
        printColoredText(username, 32);
        printColoredText("@hostname> ", 32);
        std::getline(std::cin, answer);
        fs.addToHistory(answer);
        if (answer == "history") {
            fs.history(); 
            continue;
        }
        if (answer.substr(0, 3) == "ssh") {
            size_t atPos = answer.find('@');
            if (atPos != std::string::npos) {
                std::string username = answer.substr(4, atPos - 4);
                std::string server = answer.substr(atPos + 1);
                runVirtualTerminal(server, username);
                continue;
            }
        }
        Command command(answer);
        ce.execute(command);
    }
}

void Display::runExam() {
    std::cout << "Are you sure you want to start? (yes/no): ";
    std::string input;
    std::getline(std::cin, input);
    if (input == "no") {
        std::cout << "Exiting the Linux Emulator. Goodbye!" << std::endl;
        return;
    } else if (input != "yes") {
        std::cout << "Invalid input. Exiting the Linux Emulator. Goodbye!" << std::endl;
        return;
    }
    std::cout << "Welcome! Let's begin." << std::endl;
    db.loadQuestions("q.txt");
    db.loadAnswers("a.txt");
    std::vector<Question> questions = db.getQuestions();
    std::vector<Answer> answers = db.getAnswers();
    std::string answer;
    int correctAnswers = 0;
    int numQuestions = 10;
    for (int i = 0; i < numQuestions; ++i) {
        std::cout << "Question " << i + 1 << ": " << questions[i].getQuestion() << std::endl;
        std::cout << "> ";
        std::getline(std::cin, answer);
        if (answer.empty()) {
            continue;
        }
        Command commandU(answer);
        ceUser.execute(commandU);
        std::string expectedAnswer = answers[i].getAnswer();
        answer.erase(std::remove_if(answer.begin(), answer.end(), [](unsigned char c) { return std::isspace(c); }), answer.end());
        expectedAnswer.erase(std::remove_if(expectedAnswer.begin(), expectedAnswer.end(), [](unsigned char c) { return std::isspace(c); }), expectedAnswer.end());
        Command commandMy(answers[i].getAnswer());
        std::ostringstream userOutput;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(userOutput.rdbuf());
        std::streambuf* oldCerrBuffer = std::cerr.rdbuf(userOutput.rdbuf());
        ceMy.execute(commandMy);
        std::cout.rdbuf(oldCoutBuffer);
        std::cerr.rdbuf(oldCerrBuffer);
        std::string userCommandOutput = userOutput.str();
        userOutput.str("");
        const std::vector<std::string> stateChangeCommands = {"mkdir", "touch", "mv", "cp", "rm", "rmdir"};
        if (std::find(stateChangeCommands.begin(), stateChangeCommands.end(), commandU.getName()) != stateChangeCommands.end()) {
            if (fsMy == fsUser) {
                correctAnswers++;
            } else {
                fsUser = fsMy;
            }
        } else {
            if (answer == expectedAnswer) {
                correctAnswers++;
            }
        }
    }
    int score = correctAnswers * 10;
    std::cout << "Quiz finished. You answered " << correctAnswers << " out of " << 10
              << " questions correctly. You get " << score << std::endl;
    if (score < 90) {
        std::cout << "You are failed :(" << std::endl;
    }
    else {
        std::cout << "Congratulationsss!!! You are passed." << std::endl;
    }
}

void Display::runVirtualTerminal(const std::string& server, const std::string& username) {
    std::string pas;
    std::cout << "Password: ";
    std::getline(std::cin, pas);
    if (pas != "1111") {
        return;
    }
    fsUser.clear();
    std::string command;
    std::cout << "Welcome to the virtual terminal on server " << server << " as user " << username 
    << "!" << std::endl;
    std::cout << "Enter 'exit' to logout and return to the main interface." << std::endl;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command == "exit") {
            break;
        }
        Command com(command);
        ceUser.execute(com);
    }
    std::cout << "Logged out from the server." << std::endl;
}

} // namespace LinuxEmulator

#endif // LINUX_EMULATOR_DISPLAY_H