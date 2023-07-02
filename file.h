#ifndef LINUX_EMULATOR_FILE_H
#define LINUX_EMULATOR_FILE_H

#include <string>
#include <vector>
#include <bitset>
#include <iomanip>
#include <sstream>

namespace LinuxEmulator {

enum class Permission {
    None = 0,
    OthersExecute = 1,      
    OthersWrite = 2,        
    OthersRead = 4,         
    GroupExecute = 8,       
    GroupWrite = 16,        
    GroupRead = 32,         
    OwnerExecute = 64,      
    OwnerWrite = 128,       
    OwnerRead = 256        
};

inline bool operator&(Permission lhs, Permission rhs) {
    using T = std::underlying_type_t<Permission>;
    return static_cast<T>(lhs) & static_cast<T>(rhs);
}

inline Permission& operator|=(Permission& lhs, Permission rhs) {
    using T = std::underlying_type_t<Permission>;
    lhs = static_cast<Permission>(static_cast<T>(lhs) | static_cast<T>(rhs));
    return lhs;
}

constexpr Permission operator|(Permission a, Permission b) {
    return static_cast<Permission>(static_cast<int>(a) | static_cast<int>(b));
}

class File {
public:
    File();
    File(const std::string&);
    File(const std::string&, const std::string&, char*, const std::string&, const Permission&, bool);
    File& operator=(const File&);
    File(const File&);
    ~File();
    void setName(const std::string&);
    std::string getName() const;
    void setAbsolutePath(const std::string&);
    std::string getAbsolutePath() const;
    void setContent(const char*);
    char* getContent() const;
    void setFormat(const std::string&);
    std::string getFormat() const;
    int getOctalPermissions() const;
    std::string getPermissionsString() const;
    void setPermissions(bool, bool, bool, bool, bool, bool, bool, bool, bool);
    void setPermissionsFromOctal(int);
    void setPermissions(Permission);
    Permission getPermissions() const;
    void setIsDirectory(bool);
    bool getIsDirectory() const;
private:
    std::string name;
    std::string absolutePath;
    char* content;
    std::string format;
    Permission permissions;
    bool is_Directory;
};

File::File() = default;

File::File(const std::string& n) : name{n} {}

File::File(const std::string& n, const std::string& p, char* c, const std::string& f, const Permission& per, bool is_d)
    : name{n}, absolutePath{p}, content{nullptr}, format{f}, permissions{per}, is_Directory{is_d} 
{
    if (content != nullptr) {
        this->content = new char[strlen(content) + 1];
        strcpy(this->content, content);
    }
}

File::File(const File& other) : name(other.name), absolutePath(other.absolutePath), content(nullptr), format(other.format), 
    permissions(other.permissions), is_Directory(other.is_Directory) 
{
    if (other.content != nullptr) {
        this->content = new char[strlen(other.content) + 1];
        strcpy(this->content, other.content);
    }
}

File& File::operator=(const File& other) {
    if (this == &other) {
        return *this;
    }
    delete[] content;
    content = nullptr;
    name = other.name;
    absolutePath = other.absolutePath;
    format = other.format;
    permissions = other.permissions;
    is_Directory = other.is_Directory;
    if (other.content != nullptr) {
        this->content = new char[strlen(other.content) + 1];
        strcpy(this->content, other.content);
    }
    return *this;
}

File::~File() {
    delete[] content;
}

void File::setName(const std::string& n) {
    name = n;
}

std::string File::getName() const {
    return name;
}

void File::setAbsolutePath(const std::string& p) {
    absolutePath = p;
}

std::string File::getAbsolutePath() const {
    return absolutePath;
}

void File::setContent(const char* c) {
    delete[] content;
    if (c != nullptr) {
        content = new char[strlen(c) + 1];
        strcpy(content, c);
    } else {
        content = nullptr;
    }
}

char* File::getContent() const {
    return content;
}

void File::setFormat(const std::string& f) {
    format = f;
}

std::string File::getFormat() const {
    return format;
}

Permission File::getPermissions() const {
	return permissions;
}

void File::setPermissions(bool ownerRead, bool ownerWrite, bool ownerExecute,
    bool groupRead, bool groupWrite, bool groupExecute,
    bool othersRead, bool othersWrite, bool othersExecute) {
    permissions = Permission::None;

    if (ownerRead) permissions |= Permission::OwnerRead;
    if (ownerWrite) permissions |= Permission::OwnerWrite;
    if (ownerExecute) permissions |= Permission::OwnerExecute;
    if (groupRead) permissions |= Permission::GroupRead;
    if (groupWrite) permissions |= Permission::GroupWrite;
    if (groupExecute) permissions |= Permission::GroupExecute;
    if (othersRead) permissions |= Permission::OthersRead;
    if (othersWrite) permissions |= Permission::OthersWrite;
    if (othersExecute) permissions |= Permission::OthersExecute;
}

void File::setPermissions(Permission permissions) {
    this->permissions = permissions;
}

void File::setPermissionsFromOctal(int octal) {
    permissions = static_cast<Permission>(octal);
}

int File::getOctalPermissions() const {
    return static_cast<int>(permissions);
}

std::string File::getPermissionsString() const {
    const std::string permSymbols = "rwx";
    std::ostringstream permissionStr;

    if (static_cast<int>(permissions) & static_cast<int>(Permission::OwnerRead))
        permissionStr << permSymbols[0];
    else
        permissionStr << '-';

    if (static_cast<int>(permissions) & static_cast<int>(Permission::OwnerWrite))
        permissionStr << permSymbols[1];
    else
        permissionStr << '-';

    if (static_cast<int>(permissions) & static_cast<int>(Permission::OwnerExecute))
        permissionStr << permSymbols[2];
    else
        permissionStr << '-';

    if (static_cast<int>(permissions) & static_cast<int>(Permission::GroupRead))
        permissionStr << permSymbols[0];
    else
        permissionStr << '-';

    if (static_cast<int>(permissions) & static_cast<int>(Permission::GroupWrite))
        permissionStr << permSymbols[1];
    else
        permissionStr << '-';

    if (static_cast<int>(permissions) & static_cast<int>(Permission::GroupExecute))
        permissionStr << permSymbols[2];
    else
        permissionStr << '-';

    if (static_cast<int>(permissions) & static_cast<int>(Permission::OthersRead))
        permissionStr << permSymbols[0];
    else
        permissionStr << '-';

    if (static_cast<int>(permissions) & static_cast<int>(Permission::OthersWrite))
        permissionStr << permSymbols[1];
    else
        permissionStr << '-';

    if (static_cast<int>(permissions) & static_cast<int>(Permission::OthersExecute))
        permissionStr << permSymbols[2];
    else
        permissionStr << '-';

    return permissionStr.str();
}

void File::setIsDirectory(bool i) {
    is_Directory = i;
}

bool File::getIsDirectory() const {
    return is_Directory;
}

} // namespace LinuxEmulator

#endif // LINUX_EMULATOR_FILE_H