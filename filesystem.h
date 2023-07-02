#ifndef LINUX_EMULATOR_FILESYSTEM_H
#define LINUX_EMULATOR_FILESYSTEM_H

#include "gtree.h"
#include "commandvalidator.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

namespace LinuxEmulator {

class FileSystem {
public:
    FileSystem();
    void help();
    void pwd();
    void ls();
    void lsDetailed();
    void lsSortedByTime();
    void lsLongFormat();
    void cd(const std::string&);
    void createFile(const std::string&);
    void createDirectory(const std::string&);
    void readFile(const std::string&);
    void writeFile(const std::string&);
    void copyFile(const std::string&, const std::string&);
    void moveFile(const std::string&, const std::string&);
    void renameItem(const std::string&, const std::string&);
    void deleteFile(const std::string&);
    void updateChildPaths(Node*, const std::string&, const std::string&);
    Node* findNode(const std::string&) const;
    std::string getCurrentDirectory() const;
    void setCurrentDirectory(const std::string&);
    void chmod(const std::string&, const std::string&);
    void addToHistory(const std::string&);
    void clearCommandHistory();
    std::vector<std::string> getCommandHistory() const;
    void history();
    void clear();
    void head(int, const std::string&);
    void tail(int, const std::string&);
    void file(const std::string&);
    void ln(const std::string&, const std::string&);
    void wc(const std::string&);
    bool operator==(const FileSystem& other) const {
        return getCurrentDirectory() == other.getCurrentDirectory() && tree == other.tree;
    }
    std::string getFullPath(Node*);
    void deleteDirectoryContents(Node*);
private:
    GeneralTree tree;
    std::string currentDirectory;
    std::vector<std::string> commandHistory;
    std::string previousDirectory;
};

std::vector<std::string> splitPath(const std::string& path) {
    std::vector<std::string> components;
    std::string component;
    std::istringstream iss(path);
    while (std::getline(iss, component, '/')) {
        if (!component.empty()) {
            components.push_back(component);
        }
    }
    return components;
}

FileSystem::FileSystem() {
    File rootFile("/", "/", nullptr, "", Permission::OwnerRead | Permission::OwnerWrite | Permission::OwnerExecute | 
                  Permission::GroupRead | Permission::GroupWrite | Permission::GroupExecute | 
                  Permission::OthersRead | Permission::OthersWrite | Permission::OthersExecute, 
                  true);
    Node* rootNode = new Node(rootFile);
    rootNode->parent = nullptr;
    tree.setRootData(rootFile);
    tree.setRoot(rootNode);
    currentDirectory = "/";
    previousDirectory = "/";
}

void FileSystem::addToHistory(const std::string& command) {
    std::string commandToAdd = command;
    commandToAdd.erase(std::remove(commandToAdd.begin(), commandToAdd.end(), '\n'), commandToAdd.end());
    commandHistory.push_back(commandToAdd);
}

void FileSystem::clearCommandHistory() {
    commandHistory.clear();
}

std::vector<std::string> FileSystem::getCommandHistory() const {
    return commandHistory;
}

void FileSystem::history() {
    for (int i = 0; i < commandHistory.size(); ++i) {
        std::cout << commandHistory[i] << std::endl;
    }
}

void FileSystem::file(const std::string& fileName) {
    int index = 0;
    for (int i = 0; i < fileName.size(); ++i) {
        if (fileName[i] == '.') {
            index = i;
        }
    }
    std::string format = fileName.substr(index + 1);
    std::cout << fileName << ": " << format << " file" << std::endl;
}

void FileSystem::ln(const std::string& source, const std::string& destination) {
    Node* sourceNode = findNode(source);
    Node* destinationNode = findNode(destination);
    
    if (sourceNode == nullptr) {
        std::cout << "Source item not found: " << source << std::endl;
        return;
    }
    
    if (destinationNode != nullptr) {
        std::cout << "Destination item already exists: " << destination << std::endl;
        return;
    }

    if (sourceNode->data.getIsDirectory()) {
        createDirectory(destination);
    }
    else {
        createFile(destination);
    }
}

void FileSystem::wc(const std::string& fileName) {
    Node* fileNode = findNode(fileName);
    if (!fileNode) {
        std::cout << "File not found: " << fileName << std::endl;
        return;
    }
    const std::string& content = fileNode->data.getContent();
    int lineCount = 0;
    int wordCount = 0;
    int charCount = 0;
    std::istringstream iss(content);
    std::string word;
    std::string line; 
    while (std::getline(iss, line)) {
        ++lineCount;
        std::istringstream lineIss(line);   
        while (lineIss >> word) {
            ++wordCount;
            charCount += word.length();
        }
    }
    std::cout << "Lines: " << lineCount << std::endl;
    std::cout << "Words: " << wordCount << std::endl;
    std::cout << "Characters: " << charCount << std::endl;
}

void FileSystem::clear() {
    std::cout << "\033[2J\033[1;1H";
}

void FileSystem::head(int numLines, const std::string& fileName) {
    Node* fileNode = findNode(fileName);
    if (fileNode == nullptr) {
        std::string currentDirectory = getCurrentDirectory();
        std::string filePath = currentDirectory + "/" + fileName;
        fileNode = findNode(filePath);
    }
    if (fileNode == nullptr || fileNode->data.getIsDirectory()) {
        std::cout << "File not found or the provided path is a directory." << std::endl;
        return;
    }
    std::string content = fileNode->data.getContent();
    std::vector<std::string> lines;
    std::istringstream iss(content);
    std::string line;
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    int linesToPrint = std::min(numLines, static_cast<int>(lines.size()));
    for (int i = 0; i < linesToPrint; ++i) {
        std::cout << lines[i] << std::endl;
    }
}

void FileSystem::tail(int numLines, const std::string& fileName) {
    Node* fileNode = findNode(fileName);
    if (fileNode == nullptr) {
        std::string currentDirectory = getCurrentDirectory();
        std::string filePath = currentDirectory + "/" + fileName;
        fileNode = findNode(filePath);
    }
    if (fileNode == nullptr || fileNode->data.getIsDirectory()) {
        std::cout << "File not found or the provided path is a directory." << std::endl;
        return;
    }

    std::string content = fileNode->data.getContent();
    std::vector<std::string> lines;
    std::istringstream iss(content);
    std::string line;
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    int startLine = std::max(static_cast<int>(lines.size()) - numLines, 0);
    for (int i = startLine; i < lines.size(); ++i) {
        std::cout << lines[i] << std::endl;
    }
}

void FileSystem::chmod(const std::string& permissions, const std::string& filePath) {
    Node* fileNode = findNode(filePath);
    if (fileNode == nullptr) {
        std::cout << "File not found: " << filePath << std::endl;
        return;
    }
    int octalPermissions = std::stoi(permissions, 0, 8);
    fileNode->data.setPermissionsFromOctal(octalPermissions);
}

std::string FileSystem::getCurrentDirectory() const {
    return currentDirectory;
}

void FileSystem::setCurrentDirectory(const std::string& c) {
    currentDirectory = c;
}

std::string normalizePath(const std::string& path) {
    if (path.empty() || path == "/") {
        return "/";
    }

    std::stringstream ss(path);
    std::string item;
    std::vector<std::string> parts;

    while (std::getline(ss, item, '/')) {
        if (item == "..") {
            if (!parts.empty()) {
                parts.pop_back();
            }
        } else if (item != ".") {
            parts.push_back(item);
        }
    }

    std::string normalized;
    for (const auto& part : parts) {
        normalized += "/" + part;
    }

    if (normalized.empty()) {
        normalized = "/";
    }

    return normalized;
}

Node* findChildNode(Node* parent, const std::string& name) {
    for (Node* child : parent->children) {
        if (child->data.getName() == name) {
            return child;
        }
    }
    return nullptr; 
}

Node* FileSystem::findNode(const std::string& path) const {
    std::string normalizedPath = normalizePath(path);
    std::vector<std::string> parts = splitPath(normalizedPath);

    Node* currentNode = tree.getRoot();
    for (const auto& part : parts) {
        currentNode = findChildNode(currentNode, part);
        if (currentNode == nullptr) {
            return nullptr; // Path not found
        }
    }
    return currentNode;
}

std::string FileSystem::getFullPath(Node* node) {
    if (node == tree.getRoot()) {
        return "/";
    }

    std::vector<std::string> dirs;
    Node* currentNode = node;
    while (currentNode != tree.getRoot()) {
        dirs.push_back(currentNode->data.getName());
        currentNode = currentNode->parent;
    }

    std::string fullPath;
    for (auto it = dirs.rbegin(); it != dirs.rend(); ++it) {
        fullPath += "/";
        fullPath += *it;
    }
    return fullPath;
}

void FileSystem::pwd() {
    std::string fullPath = getFullPath(findNode(currentDirectory));
    std::cout << fullPath << std::endl;
}

void FileSystem::help() {
    CommandValidator cv;
    for (const auto& option : cv.getValidOptions()) {
        std::cout << option.first << ": ";
        for (const auto& value : option.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

void FileSystem::cd(const std::string& directoryPath) {
    std::string newPath = directoryPath;
    std::string homeDirectory = "/home/username";
    if (newPath == "~") {
        newPath = homeDirectory;
    } else if (newPath == "-") {
        std::swap(currentDirectory, previousDirectory); // Swap current and previous directories
        std::cout << "Current directory changed to: " << currentDirectory << std::endl;
        return;
    } else if (newPath == "..") {
        size_t lastSlashIndex = currentDirectory.find_last_of('/');
        if (lastSlashIndex != std::string::npos) {
            newPath = currentDirectory.substr(0, lastSlashIndex);
        } else {
            std::cout << "Error: Cannot go up from root directory." << std::endl;
            return;
        }
    } else if (newPath == ".") {
        return;
    }
    Node* directoryNode = findNode(newPath);
    if (directoryNode == nullptr) {
        std::cout << "No such file or directory." << std::endl;
        return;
    }
    if (!directoryNode->data.getIsDirectory()) {
        std::cout << "Error: Not a directory." << std::endl;
        return;
    }
    previousDirectory = currentDirectory;
    currentDirectory = newPath;
    std::cout << "Current directory changed to: " << currentDirectory << std::endl;
}

void FileSystem::createFile(const std::string& filePath) {
    if (filePath.empty()) {
        std::cout << "Invalid file path. Please provide a valid file path or filename." << std::endl;
        return;
    }
    std::size_t found = filePath.find_last_of("/");
    std::string directoryPath;
    std::string fileName;
    if (found != std::string::npos) {
        directoryPath = filePath.substr(0, found);
        fileName = filePath.substr(found + 1);
    } else {
        directoryPath = currentDirectory;
        fileName = filePath;
    }

    Node* parentNode = findNode(directoryPath);
    if (parentNode == nullptr) {
        std::cout << "Directory does not exist. File creation failed." << std::endl;
        return;
    }
    for (const auto& child : parentNode->children) {
        if (child->data.getName() == fileName && !child->data.getIsDirectory()) {
            std::cout << "File with the same name already exists in the directory. File creation failed." << std::endl;
            return;
        }
    }
    File file(fileName, filePath, nullptr, "", Permission::OwnerRead | Permission::OwnerWrite | Permission::OwnerExecute | 
                  Permission::GroupRead | Permission::GroupWrite | Permission::GroupExecute | 
                  Permission::OthersRead | Permission::OthersWrite | Permission::OthersExecute, false);
    tree.insert(parentNode->data, file);
}

void FileSystem::createDirectory(const std::string& directoryName) {
    std::string currentPath = currentDirectory;
    std::size_t slashPos = directoryName.find('/');
    if (slashPos != std::string::npos) {
        std::string path = directoryName.substr(0, slashPos);
        std::string name = directoryName.substr(slashPos + 1);
        currentPath += "/" + path;
        Node* parentNode = findNode(currentPath);
        if (parentNode == nullptr || !parentNode->data.getIsDirectory()) {
            std::cout << "Parent directory does not exist." << std::endl;
            return;
        }
        File newDirectory(name, currentPath + "/" + name, nullptr, "", Permission::OwnerRead | Permission::OwnerWrite | Permission::OwnerExecute | 
                  Permission::GroupRead | Permission::GroupWrite | Permission::GroupExecute | 
                  Permission::OthersRead | Permission::OthersWrite | Permission::OthersExecute, true);
        Node* newDirectoryNode = new Node(newDirectory);
        tree.insert(parentNode, newDirectoryNode);
        std::cout << "Directory created successfully." << std::endl;
    } else {
        std::string parentDirectoryPath = currentPath + "/" + directoryName;
        Node* parentDirectoryNode = findNode(parentDirectoryPath);
        if (parentDirectoryNode != nullptr) {
            std::cout << "Directory already exists." << std::endl;
            return;
        }
        File newDirectory(directoryName, parentDirectoryPath, nullptr, "", Permission::OwnerRead | Permission::OwnerWrite | Permission::OwnerExecute | 
                  Permission::GroupRead | Permission::GroupWrite | Permission::GroupExecute | 
                  Permission::OthersRead | Permission::OthersWrite | Permission::OthersExecute, true);
        Node* parentNode = findNode(currentPath);
        if (parentNode == nullptr || !parentNode->data.getIsDirectory()) {
            std::cout << "Parent directory does not exist." << std::endl;
            return;
        }
        Node* newDirectoryNode = new Node(newDirectory);
        tree.insert(parentNode, newDirectoryNode);
        std::cout << "Created" << std::endl;
    }
}

void FileSystem::readFile(const std::string& fileName) {
    Node* fileNode = findNode(fileName);
    if (fileNode == nullptr) {
        std::string currentDirectory = getCurrentDirectory();
        std::string filePath = currentDirectory + "/" + fileName;
        fileNode = findNode(filePath);
    }
    if (fileNode == nullptr || fileNode->data.getIsDirectory()) {
        std::cout << "File not found or the provided path is a directory." << std::endl;
        return;
    }
    std::cout << fileNode->data.getContent() << std::endl;
}

void FileSystem::writeFile(const std::string& fileName) {
    std::cout << "If you end typing press !q" << std::endl;
    Node* fileNode = findNode(fileName);
    if (fileNode == nullptr) {
        std::string currentDirectory = getCurrentDirectory();
        std::string filePath = currentDirectory + "/" + fileName;
        fileNode = findNode(filePath);
    }
    if (fileNode == nullptr || fileNode->data.getIsDirectory()) {
        std::cout << "File not found or the provided path is a directory." << std::endl;
        return;
    }
    std::string input;
    std::ostringstream contentStream;
    while (getline(std::cin, input) && input != "!q") {
        contentStream << input << '\n';
    }
    std::string content = contentStream.str();
    fileNode->data.setContent(content.c_str());
}

void FileSystem::copyFile(const std::string& source, const std::string& destination) {
    Node* sourceNode = findNode(source);
    Node* destinationNode = findNode(destination);

    if (sourceNode == nullptr) {
        std::string sourcePath = getCurrentDirectory() + "/" + source;
        sourceNode = findNode(sourcePath);
    }
    if (sourceNode == nullptr) {
        std::cout << "Source path not found." << std::endl;
        return;
    }
    if (destinationNode != nullptr) {
        std::cout << "Destination path already exists." << std::endl;
        return;
    }
    std::size_t found = destination.find_last_of("/");
    if (found == std::string::npos) {
        std::cout << "Invalid destination path. Please provide the full path including the directory." << std::endl;
        return;
    }
    std::string destinationDirectory = destination.substr(0, found);
    std::string destinationName = destination.substr(found + 1);
    Node* destinationParentNode = findNode(destinationDirectory);
    if (destinationParentNode == nullptr) {
        std::cout << "Destination directory does not exist." << std::endl;
        return;
    }
    Permission permissions = Permission::OwnerRead | Permission::OwnerWrite | Permission::GroupRead | Permission::OthersRead;
    destinationNode = new Node(File(destinationName, destination, sourceNode->data.getContent(), sourceNode->data.getFormat(), permissions, false));
    destinationParentNode->addChild(destinationNode);
    std::cout << "File copied successfully." << std::endl;
}


void FileSystem::moveFile(const std::string& source, const std::string& destination) {
    Node* sourceNode = findNode(source);
    Node* destinationNode = findNode(destination);
    if (sourceNode == nullptr) {
        std::cout << "Source path not found." << std::endl;
        return;
    }
    if (destinationNode != nullptr && destinationNode->data.getIsDirectory()) {
        std::string destinationPath = destination + "/" + sourceNode->data.getName();
        sourceNode->data.setAbsolutePath(destinationPath);
        if (sourceNode->parent != nullptr) {
            sourceNode->parent->removeChild(sourceNode); // Remove from the previous parent
        }
        destinationNode->addChild(sourceNode);
        sourceNode->parent = destinationNode;
        std::cout << "File or directory moved successfully." << std::endl;
    } else {
        std::size_t found = destination.find_last_of("/");
        if (found == std::string::npos) {
            std::cout << "Invalid destination path. Please provide the full path including the directory." << std::endl;
            return;
        }
        std::string destinationDirectory = destination.substr(0, found);
        std::string destinationName = destination.substr(found + 1);
        Node* destinationParentNode = findNode(destinationDirectory);
        if (destinationParentNode == nullptr || !destinationParentNode->data.getIsDirectory()) {
            std::cout << "Destination directory does not exist." << std::endl;
            return;
        }
        Node* existingNode = findNode(destination);
        if (existingNode != nullptr) {
            std::cout << "A file or directory already exists at the destination path." << std::endl;
            return;
        }
        sourceNode->data.setName(destinationName);
        sourceNode->data.setAbsolutePath(destination);
        if (sourceNode->parent != nullptr) {
            sourceNode->parent->removeChild(sourceNode); // Remove from the previous parent
        }
        destinationParentNode->addChild(sourceNode);
        sourceNode->parent = destinationParentNode;
        std::cout << "File or directory moved successfully." << std::endl;
        if (sourceNode->parent != destinationParentNode) {
            sourceNode->parent->removeChild(sourceNode); // Remove from the source parent
        }
    }
}

void FileSystem::renameItem(const std::string& itemPath, const std::string& newName) {
    Node* itemNode = findNode(itemPath);
    if (itemNode == nullptr) {
        std::cout << "Item not found." << std::endl;
        return;
    }
    std::string parentPath = itemNode->data.getAbsolutePath();
    std::size_t found = parentPath.find_last_of("/");
    std::string newPath = parentPath.substr(0, found + 1) + newName;
    itemNode->data.setName(newName);
    itemNode->data.setAbsolutePath(newPath);
    std::cout << "Item renamed successfully." << std::endl;
}

void FileSystem::deleteFile(const std::string& filePath) {
    Node* fileNode = findNode(filePath);
    if (fileNode == nullptr) {
        std::cout << "File or directory not found." << std::endl;
        return;
    }
    if (fileNode->data.getIsDirectory()) {
        deleteDirectoryContents(fileNode);
    }
    tree.remove(fileNode);
    std::cout << "File or directory deleted successfully." << std::endl;
}

void FileSystem::deleteDirectoryContents(Node* directoryNode) {
    for (Node* child : directoryNode->children) {
        if (child->data.getIsDirectory()) {
            deleteDirectoryContents(child);
        }
        tree.remove(child);
    }
}

void printColoredText(const std::string& text, int colorCode) {
    std::cout << "\033[" << colorCode << "m" << text << "\033[0m";
}

void FileSystem::ls() {
    Node* currentNode = findNode(currentDirectory);
    if (currentNode == nullptr || !currentNode->data.getIsDirectory()) {
        std::cout << "Current directory not found." << std::endl;
        return;
    }
    std::cout << "Listing directory: " << currentDirectory << std::endl;
    for (Node* child : currentNode->children) {
        if (child->data.getIsDirectory()) {
            printColoredText(child->data.getName(), 34);
            std::cout << std::endl;
        }
        else {
            printColoredText(child->data.getName(), 32);
            std::cout << std::endl;
        }
    }
}

void FileSystem::lsDetailed() {
    Node* currentNode = findNode(currentDirectory);
    if (currentNode == nullptr || !currentNode->data.getIsDirectory()) {
        std::cout << "Current directory not found." << std::endl;
        return;
    }
    std::cout << "Detailed listing of directory: " << currentDirectory << std::endl;
    for (Node* child : currentNode->children) {
        if (child->data.getIsDirectory()) {
            printColoredText(child->data.getName(), 34);
        }
        else {
            printColoredText(child->data.getName(), 32);
        }
        std::cout << " " << child->data.getPermissionsString();
        std::cout << std::endl;
    }
}

void FileSystem::lsSortedByTime() {
    Node* currentNode = findNode(currentDirectory);
    if (currentNode == nullptr || !currentNode->data.getIsDirectory()) {
        std::cout << "Current directory not found." << std::endl;
        return;
    }
    std::time_t currentTime = std::time(nullptr);
    std::cout << "Listing directory sorted by time: " << currentDirectory << std::endl;
    std::vector<Node*> sortedChildren = currentNode->children;
    std::sort(sortedChildren.begin(), sortedChildren.end(), [](Node* a, Node* b) {
        return a->data.getName() < b->data.getName();
    });
    for (Node* child : sortedChildren) {
        if (child->data.getIsDirectory()) {
            printColoredText(child->data.getName(), 34);
            std::cout << " " << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S") << std::endl;
        } else {
            printColoredText(child->data.getName(), 32);
            std::cout << " " << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S") << std::endl;
        }
    }
}

void FileSystem::lsLongFormat() {
    Node* currentNode = findNode(currentDirectory);
    if (currentNode == nullptr || !currentNode->data.getIsDirectory()) {
        std::cout << "Current directory not found." << std::endl;
        return;
    }
    std::cout << "Long format listing of directory: " << currentDirectory << std::endl;
    for (Node* child : currentNode->children) {
        std::cout << (child->data.getIsDirectory() ? "d" : "-");
        std::cout << child->data.getPermissionsString() << " ";
        if (child->data.getIsDirectory()) {
            printColoredText(child->data.getName(), 34);
            std::cout << std::endl;
        }
        else {
            printColoredText(child->data.getName(), 32);
            std::cout << std::endl;
        }
    }
}

} // namespace LinuxEmulator

#endif  // LINUX_EMULATOR_FILESYSTEM_H