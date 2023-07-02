#ifndef LINUX_EMULATOR_COMMAND_H
#define LINUX_EMULATOR_COMMAND_H

#include <string>
#include <vector>
#include <map>
#include <sstream>

namespace LinuxEmulator {

class Command {
public:
    Command();
    Command(const std::string&);
    void setName(const std::string&);
    std::string getName() const;
    void setArguments(const std::vector<std::string>&);
    std::vector<std::string> getArguments() const;
    void setOptions(const std::map<std::string, std::vector<std::string>>&);
    std::map<std::string, std::vector<std::string>> getOptions() const;
	std::vector<std::string> splitCommand(const std::string&, char);
	bool isOption(const std::string&) const;
private:
    std::string name;
    std::vector<std::string> arguments;
    std::map<std::string, std::vector<std::string>> options;
};

Command::Command() = default;

Command::Command(const std::string& fullCommand) {
    std::vector<std::string> commandParts = splitCommand(fullCommand, ' ');
    name = commandParts[0];
    for (size_t i = 1; i < commandParts.size(); ++i) {
        if (commandParts[i][0] == '-') {
            std::string optionName = commandParts[i];
            std::vector<std::string> optionArgs;
            while (++i < commandParts.size() && commandParts[i][0] != '-' && commandParts[i][0] != '/') {
                optionArgs.push_back(commandParts[i]);
            }
            --i;
            options[optionName] = optionArgs;
        } else {
            arguments.push_back(commandParts[i]);
        }
    }
}


std::vector<std::string> Command::splitCommand(const std::string& str, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string part;
    while (std::getline(ss, part, delimiter)) {
        parts.push_back(part);
    }
    return parts;
}

bool Command::isOption(const std::string& str) const {
    return str.size() > 1 && str[0] == '-';
}

void Command::setName(const std::string& n) {
    name = n;
}

std::string Command::getName() const {
    return name;
}

void Command::setArguments(const std::vector<std::string>& args) {
    arguments = args;
}

std::vector<std::string> Command::getArguments() const {
    return arguments;
}

void Command::setOptions(const std::map<std::string, std::vector<std::string>>& opts) {
    options = opts;
}

std::map<std::string, std::vector<std::string>> Command::getOptions() const {
    return options;
}

} // namespace LinuxEmulator

#endif // LINUX_EMULATOR_COMMAND_H