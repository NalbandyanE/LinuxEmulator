#ifndef LINUX_EMULATOR_ANOTHERCOMMANDS_H
#define LINUX_EMULATOR_ANOTHERCOMMANDS_H

#include "user.h"

#include <fstream>
#include <ctime>
#include <iomanip>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

namespace LinuxEmulator {

class AnotherCommands {
public:
    void date();
    void cal();
    void df();
    void free();
    void echo(const std::string&);
    void ps();
    void top();
    void jobs();
    void whatis(const std::string&);
};

void AnotherCommands::whatis(const std::string& keyword) {
    if (keyword == "ls") {
    	std::cout << "list directory contents" << std::endl;
    }
    else if (keyword == "cp") {
    	std::cout << "copy files and directories" << std::endl;
    }
    else if (keyword == "mv") {
    	std::cout << "move (rename) files" << std::endl;
    }
    else {
    	std::cout << "nothing appropriate." << std::endl;
    }
}

void AnotherCommands::ps() {
    DIR* procDir = opendir("/proc");
    if (procDir == NULL) {
        perror("opendir");
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(procDir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            std::cout << entry->d_name << std::endl;
        }
    }
    closedir(procDir);
}

void AnotherCommands::top() {
    DIR* procDir = opendir("/proc");
    if (procDir == nullptr) {
        perror("opendir");
        return;
    }
    struct dirent* entry;
    std::cout << "PID   TTY      TIME     CMD" << std::endl;
    while ((entry = readdir(procDir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            char* end;
            long pid = strtol(entry->d_name, &end, 10);
            if (*end == '\0') {
                std::string statFilePath = "/proc/" + std::string(entry->d_name) + "/stat";
                std::ifstream statFile(statFilePath);
                if (!statFile) {
                    perror("ifstream");
                    continue;
                }
                std::string statLine;
                std::getline(statFile, statLine);
                std::istringstream iss(statLine);
                std::string pid, comm, state, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt, majflt, cmajflt,
                    utime, stime, cutime, cstime, priority, nice, num_threads, itrealvalue, starttime;
                iss >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt
                    >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads
                    >> itrealvalue >> starttime;
                std::cout << pid << "   " << tty_nr << "   " << utime << ":" << stime << "   " << comm << std::endl;
            }
        }
    }
    closedir(procDir);
}

void AnotherCommands::jobs() {
    DIR* procDir = opendir("/proc");
    if (procDir == nullptr) {
        perror("opendir");
        return;
    }
    struct dirent* entry;
    std::cout << "PID   STATUS     CMD" << std::endl;
    while ((entry = readdir(procDir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            char* end;
            long pid = strtol(entry->d_name, &end, 10);
            if (*end == '\0') {
                std::string statusFilePath = "/proc/" + std::string(entry->d_name) + "/status";
                std::ifstream statusFile(statusFilePath);
                if (!statusFile) {
                    perror("ifstream");
                    continue;
                }
                std::string line;
                std::string state;
                while (std::getline(statusFile, line)) {
                    if (line.substr(0, 6) == "State:") {
                        state = line.substr(7);
                        break;
                    }
                }
                std::string cmdlineFilePath = "/proc/" + std::string(entry->d_name) + "/cmdline";
                std::ifstream cmdlineFile(cmdlineFilePath);
                if (!cmdlineFile) {
                    perror("ifstream");
                    continue;
                }
                std::string cmd;
                std::getline(cmdlineFile, cmd);
                std::cout << pid << "   " << state << "   " << cmd << std::endl;
            }
        }
    }
    closedir(procDir);
}

void AnotherCommands::echo(const std::string& text) {
    std::cout << text << " ";
}

void AnotherCommands::date() {
    time_t tt;
    struct tm* ti;
    time(&tt);
    ti = localtime(&tt);
    std::cout << asctime(ti);
}

void AnotherCommands::cal() {
    time_t now = time(0);
    tm* currentDate = localtime(&now);
    int month = currentDate->tm_mon + 1;  
    int year = currentDate->tm_year + 1900;  
    int daysInMonth;
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            daysInMonth = 29;
        } else {
            daysInMonth = 28;
        }
    }
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 ||
             month == 10 || month == 12) {
        daysInMonth = 31;
    }
    else {
        daysInMonth = 30;
    }
    std::cout << "  " << std::setw(12) << std::left << "   Month: " << month << std::endl;
    std::cout << "  " << std::setw(12) << std::left << "   Year: " << year << std::endl;
    std::cout << "  ---------------------------" << std::endl;
    std::cout << "  Sat  Sun  M  Tu  W  Th  F" << std::endl;
    tm firstDay;
    firstDay.tm_year = year - 1900;
    firstDay.tm_mon = month - 1;
    firstDay.tm_mday = 1;
    mktime(&firstDay);
    int weekday = firstDay.tm_wday;  
    for (int day = 1; day <= daysInMonth + weekday; day++) {
        if (day <= weekday) {
            std::cout << "\t";
        } else {
            std::cout << std::setw(4) << std::right << day - weekday;
        }
        if ((day - weekday) % 7 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void AnotherCommands::df() {
	std::cout << "FileSystem     1K-blocks      Used Available Use% Mounted on\n"
	"rootfs         944858108 229899504 714958604  25% /\n"
	"none           944858108 229899504 714958604  25% /dev\n"
	"none           944858108 229899504 714958604  25% /run\n"
	"none           944858108 229899504 714958604  25% /run/lock\n"
	"none           944858108 229899504 714958604  25% /run/shm\n"
	"none           944858108 229899504 714958604  25% /run/user\n"
	"tmpfs          944858108 229899504 714958604  25% /sys/fs/cgroup\n"
	"C:/            944858108 229899504 714958604  25% /mnt/c\n";
}

void AnotherCommands::free() {
	std::cout << "            total        used        free      shared  buff/cache   available\n"
	"Mem:       12242208     6622892     5389964       17720      229352     5485584\n"
	"Swap:      37748736      123128    37625608                                    \n";
}

} // namespace LinuxEmulator

#endif // LINUX_EMULATOR_ANOTHERCOMMANDS_H
