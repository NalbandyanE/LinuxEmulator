#ifndef LINUX_EMULATOR_ANSWER_H
#define LINUX_EMULATOR_ANSWER_H

#include <string>

namespace LinuxEmulator {

class Answer {
public:
	Answer();
	Answer(const std::string&);
	void setAnswer(const std::string&);
	std::string getAnswer() const;
	bool operator==(const Answer& other) const {
        return answer == other.answer;
    }
private:
	std::string answer;
};

Answer::Answer() = default;

Answer::Answer(const std::string& a) : answer{a} {}

void Answer::setAnswer(const std::string& a) {
	answer = a;
}

std::string Answer::getAnswer() const {
	return answer;
}

} // namespace LinuxEmulator

#endif // LINUX_EMULATOR_ANSWER_H