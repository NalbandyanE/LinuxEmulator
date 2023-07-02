#ifndef LINUX_EMULATOR_USER_H
#define LINUX_EMULATOR_USER_H

#include <string>
#include <vector>

namespace LinuxEmulator {

class User {
public:
	User();
	User(const std::string&, const std::string&);
	void setName(const std::string&);
	std::string getName() const;
	void setPassword(const std::string&);
	std::string getPassword() const;
	int getUid() const;
	int getGid() const;
	void useradd(const std::string&);
    	User getUserByUsername(const std::string&) const;
    	void passwd();
    	void id();
private:
	std::string name;
	std::string password;
	std::string server;
	int uid;
	int gid;
	std::vector<User> users;
};

User::User() = default;

User::User(const std::string& u, const std::string& p) : name{u}, password{p}, server{"192.168.1.2"}, 
	uid{100}, gid{100} {}

void User::setName(const std::string& n) {
	name = n;
}

std::string User::getName() const {
	return name;
}

int User::getUid() const {
	return uid;
}

int User::getGid() const {
	return gid;
}

void User::setPassword(const std::string& p) {
	password = p;
}

std::string User::getPassword() const {
	return password;
}

void User::id() {
    	std::cout << "uid=" << uid << "(" << name << ") gid=" << gid << "(" << name << ")" << std::endl;
}

User User::getUserByUsername(const std::string& username) const {
   	for (int i = 0; i < users.size(); ++i) {
        	if (users[i].getName() == username) {
            		return users[i];
        	}
    	}
    	return User();
}

void User::useradd(const std::string& username) {
    	User newUser(username, "1111");
    	users.push_back(newUser);
    	std::cout << "User created successfully" << std::endl;
}

void User::passwd() {
    	std::string currentPassword;
    	std::cout << "Enter current password: ";
    	std::getline(std::cin,currentPassword);
    	if (currentPassword != getPassword()) {
        	std::cout << "Incorrect current password. Password change failed." << std::endl;
        	return;
    	}
    	std::string newPassword;
    	std::string confirmPassword;
    	std::cout << "Enter new password: ";
    	std::getline(std::cin,newPassword);
    	std::cout << "Confirm new password: ";
    	std::getline(std::cin,confirmPassword);
    	if (newPassword.size() < 4) {
        	std::cout << "Invalid new password. Password change failed." << std::endl;
        	return;
    	}
    	if (newPassword != confirmPassword) {
        	std::cout << "New password and confirm password do not match. Password change failed." << std::endl;
        	return;
    	}
    	setPassword(newPassword);
    	std::cout << "Password changed successfully." << std::endl;
}

} // namespace LinuxEmulator

#endif // LINUX_EMULATOR_USER_H
