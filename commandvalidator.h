#ifndef LINUX_EMULATOR_COMMANDVALIDATOR_H
#define LINUX_EMULATOR_COMMANDVALIDATOR_H

#include "command.h"

#include <map>
#include <algorithm>
#include <regex>

namespace LinuxEmulator {

class CommandValidator {
public:
    CommandValidator();
    CommandValidator(const Command&);
    bool isValidCommand(const Command&);
    Command getCommand() const;
    std::map<std::string, std::vector<std::string>> getValidOptions() const;
private:
    Command command;
    std::vector<std::string> validCommands = {
        "date",
        "cal",
        "df",
        "free",
        "mkdir",
        "ls",
        "pwd",
        "cd",
        "touch",
        "cat",
        "vim",
        "file",
        "mv",
        "cp",
        "rm",
        "less",
        "rmdir",
        "ln",
        "wc",
        "head",
        "tail",
        "echo",
        "history",
        "clear",
        "passwd",
        "id",
        "help",
        "ssh",
        "useradd",
        "chmod",
        "ps",
        "top",
        "jobs",
        "whatis"
    };
    std::map<std::string, std::vector<std::string>> validOptions = {
        {"cal", {}},
        {"date", {}},
        {"df", {}},
        {"free", {}},
        {"mkdir", {}},
        {"ls", {"-l", "-la", "-lt"}},
        {"pwd", {}},
        {"cd", {"..", ".", "~", "-"}},
        {"touch", {}},
        {"cat", {}},
        {"vim", {}},
        {"file", {}},
        {"mv", {}},
        {"cp", {}},
        {"rm", {}},
        {"less", {}},
        {"ln", {"-s"}},
        {"wc", {}},
        {"head", {}},
        {"tail", {}},
        {"echo", {}},
        {"history", {}},
        {"clear", {}},
        {"id", {}},
        {"passwd", {}},
        {"help", {}},
        {"ssh", {}},
        {"useradd", {}},
        {"chmod", {}},
        {"ps", {}},
        {"top", {}},
        {"jobs", {}},
        {"whatis", {}}
    };
};

CommandValidator::CommandValidator() = default;

CommandValidator::CommandValidator(const Command& com) : command(com) {}

Command CommandValidator::getCommand() const {
    return command;
}

std::map<std::string, std::vector<std::string>> CommandValidator::getValidOptions() const {
    return validOptions;
}

bool CommandValidator::isValidCommand(const Command& com) {
    std::string commandName = com.getName();
    if (std::find(validCommands.begin(), validCommands.end(), commandName) == validCommands.end()) {
        return false;
    }
    const std::map<std::string, std::vector<std::string>>& options = com.getOptions();
    const std::vector<std::string>& validOpts = validOptions[commandName];
    for (const auto& pair : options) {
        const std::vector<std::string>& opts = pair.second;
        for (const std::string& opt : opts) {
            if (std::find(validOpts.begin(), validOpts.end(), opt) == validOpts.end()) {
                return false;
            }
        }
    }
    const std::vector<std::string>& arguments = com.getArguments();
    return true;
}

} // namespace LinuxEmulator

#endif // LINUX_EMULATOR_COMMANDVALIDATOR_H