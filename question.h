#ifndef LINUX_EMULATOR_QUESTION_H
#define LINUX_EMULATOR_QUESTION_H

#include <string>
#include <cstdlib>

namespace LinuxEmulator {

class Question {
public:
	Question();
	Question(const std::string&);
	void setQuestion(const std::string&);
	std::string getQuestion() const;
private:
	std::string question;
};

Question::Question() = default;

Question::Question(const std::string& q) : question{q} {}

void Question::setQuestion(const std::string& q) {
	question = q;
}

std::string Question::getQuestion() const {
	return question;
}

} // namespace LinuxEmulator

#endif // LINUX_EMULATOR_QUESTION_H