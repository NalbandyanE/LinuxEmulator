#ifndef LINUX_EMULATOR_COMMANDEXECUTOR_H
#define LINUX_EMULATOR_COMMANDEXECUTOR_H

#include "commandvalidator.h"
#include "filesystem.h"
#include "user.h"
#include "anothercommands.h"

namespace LinuxEmulator {

class CommandExecutor {
public:
	CommandExecutor();
	CommandExecutor(const Command&);
	CommandExecutor(const FileSystem&);
    CommandExecutor(const User&);
    CommandExecutor(const FileSystem&, const User&);
	void execute(const Command&);
private:
	Command command;
	FileSystem fs;
    User u;
};

CommandExecutor::CommandExecutor() = default;

CommandExecutor::CommandExecutor(const Command& c) : command{c} {}

CommandExecutor::CommandExecutor(const FileSystem& f) : fs{f} {}

CommandExecutor::CommandExecutor(const User& us) : u{us} {}

CommandExecutor::CommandExecutor(const FileSystem& f, const User& us) : fs{f}, u{us} {}

void CommandExecutor::execute(const Command& com) {
	CommandValidator validator;
    AnotherCommands a;
	if (!validator.isValidCommand(com)) {
        std::cout << com.getName() << ": command not found" << std::endl;
        return;
    }
    if (com.getName() == "mkdir") {
        std::vector<std::string> arguments = com.getArguments();
        for (const std::string& arg : arguments) {
            fs.createDirectory(arg);
        }
    } else if(com.getName() == "date") {
        a.date();
    } else if(com.getName() == "cal") {
        a.cal();
    } else if(com.getName() == "df") {
        a.df();
    } else if(com.getName() == "free") {
        a.free();
    } else if(com.getName() == "help") {
        fs.help();
    } else if (com.getName() == "cd") {
        std::vector<std::string> arguments = com.getArguments();
        if (arguments.empty()) {
            fs.cd("/");
        } else {
            fs.cd(arguments[0]);
        }
    } else if (com.getName() == "pwd") {
        fs.pwd();
    } else if (com.getName() == "ls") {
        const std::map<std::string, std::vector<std::string>>& options = com.getOptions();
        auto isOptionPresent = [&options](const std::string& option) {
            for (const auto& optionPair : options) {
                if (optionPair.first == option) {
                    return true;
                }
            }
            return false;
        };
        
        if (isOptionPresent("-la")) {
            fs.lsDetailed();
        }
        else if (isOptionPresent("-lt")) {
            fs.lsSortedByTime();
        }
        else if (isOptionPresent("-l")) {
            fs.lsDetailed();
        }
        else {
            fs.ls();
        }
    } else if (com.getName() == "cat" || com.getName() == "less") {
        std::vector<std::string> arguments = com.getArguments();
        for (const std::string& arg : arguments) {
            fs.readFile(arg);
        }
    } else if (com.getName() == "touch") {
        std::vector<std::string> arguments = com.getArguments();
        for (const std::string& arg : arguments) {
            fs.createFile(arg);
        }
    } else if (com.getName() == "vim") {
    	std::vector<std::string> arguments = com.getArguments();
    	fs.createFile(arguments[0]);
    	fs.writeFile(arguments[0]);
    } else if (com.getName() == "rmdir") {
        std::vector<std::string> arguments = com.getArguments();
        for (const std::string& arg : arguments) {
            fs.deleteFile(arg);
        }
    } else if (com.getName() == "rm") {
        std::vector<std::string> arguments = com.getArguments();
        for (const std::string& arg : arguments) {
            fs.deleteFile(arg);
        }
    } else if (com.getName() == "mv") {
        std::vector<std::string> arguments = com.getArguments();
        std::string destination = arguments.back();
        std::vector<std::string> sources(arguments.begin(), arguments.end() - 1);
        for (int i = 0; i < sources.size(); ++i) {
        	fs.moveFile(sources[i], destination);
        }
    } else if (com.getName() == "cp") {
        std::vector<std::string> arguments = com.getArguments();
        std::string destination = arguments.back();
        std::vector<std::string> sources(arguments.begin(), arguments.end() - 1);
        for (int i = 0; i < sources.size(); ++i) {
			fs.copyFile(sources[i], destination);        
		}
    } else if (com.getName() == "chmod") {
        std::vector<std::string> arguments = com.getArguments();
        fs.chmod(arguments[0], arguments[1]);
    } else if (com.getName() == "useradd") {
        std::vector<std::string> arguments = com.getArguments();
        if (arguments.size() >= 1) {
            u.useradd(arguments[0]);
        }
        else {
            std::cout << "Invalid arguments for command useradd" << std::endl;
        }
    } else if (com.getName() == "passwd") {
        u.passwd();
    } else if (com.getName() == "history") {
        fs.history();
    } else if (com.getName() == "clear") {
        fs.clear();
    } else if (com.getName() == "echo") {
        std::vector<std::string> text = com.getArguments();
        for (int i = 0 ; i < text.size(); ++i) {
            a.echo(text[i]);
        } 
        std::cout << std::endl;
    } else if (com.getName() == "id") {
        u.id();
    } else if (com.getName() == "head") {
        if (com.getArguments().size() >= 2) {
            std::string fileName = com.getArguments().at(1);
            int numLines = std::stoi(com.getArguments().at(0));  
            fs.head(numLines, fileName);
        } else {
            std::cout << "Invalid arguments for 'head' command." << std::endl;
        }
    } else if (com.getName() == "tail") {
        if (com.getArguments().size() >= 2) {
            std::string fileName = com.getArguments().at(1);
            int numLines = std::stoi(com.getArguments().at(0));  
            fs.tail(numLines, fileName);
        } else {
            std::cout << "Invalid arguments for 'tail' command." << std::endl;
        }
    } else if (com.getName() == "file") {
        std::vector<std::string> arguments = com.getArguments();
        fs.file(arguments[0]);
    } else if (com.getName() == "wc") {
        std::vector<std::string> arguments = com.getArguments();
        fs.wc(arguments[0]);
    } else if (com.getName() == "ln" || com.getName() == "ln -s") {
        fs.ln(com.getArguments().at(0), com.getArguments().at(1));
    } else if (com.getName() == "ps") {
        a.ps();
    } else if (com.getName() == "top") {
        a.top();
    } else if (com.getName() == "jobs") {
        a.jobs();
    } else if (com.getName() == "whatis") {
        std::vector<std::string> arguments = com.getArguments();
        a.whatis(arguments[0]);
    } else {
        std::cout << "Unknown command: " << com.getName() << std::endl;
    }
}

} // namespace LinuxEmulator

#endif // LINUX_EMULATOR_COMMANDEXECUTOR_H