#ifndef LINUX_EMULATOR_DATABASE_H
#define LINUX_EMULATOR_DATABASE_H

#include "question.h"
#include "answer.h"

#include <vector>
#include <fstream>
#include <unordered_map>

namespace LinuxEmulator {

class Database {
public:
	Database();
	void setQuestions(const std::vector<Question>&);
	std::vector<Question> getQuestions() const;
	void setAnswers(const std::vector<Answer>&);
	std::vector<Answer> getAnswers() const;
	void addQuestion(const Question&);
	void addAnswer(const Answer&);
	void loadQuestions(const std::string&);
	void loadAnswers(const std::string&);
private:
	std::vector<Question> questions;
	std::vector<Answer> answers;
};

Database::Database() = default;

void Database::setQuestions(const std::vector<Question>& q) {
	questions = q;
}

std::vector<Question> Database::getQuestions() const {
	return questions;
}

void Database::setAnswers(const std::vector<Answer>& a) {
	answers = a;
}

std::vector<Answer> Database::getAnswers() const {
	return answers;
}

void Database::addQuestion(const Question& q) {
	questions.push_back(q);
}

void Database::addAnswer(const Answer& a) {
	answers.push_back(a);
}

void Database::loadQuestions(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "Can not open file" << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line)) {
		Question question(line);
		addQuestion(question);
	}
	file.close();
}

void Database::loadAnswers(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "Can not open file" << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line)) {
		Answer answer(line);
		addAnswer(answer);
	}
	file.close();
}


} // namespace LinuxEmulator

#endif // LINUX_EMULATOR_DATABASE_H