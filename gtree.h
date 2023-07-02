#ifndef LINUX_EMULATOR_GTREE_H
#define LINUX_EMULATOR_GTREE_H

#include "file.h"

#include <iostream>
#include <vector>

namespace LinuxEmulator {

struct Node {
    File data;
    Node* parent;
    std::vector<Node*> children;
    Node(const File&);
    Node* getParent() const;
    void addChild(Node*);
    ~Node();
    bool operator==(const Node&) const;
    void removeChild(Node*);
};

class GeneralTree {
private:
    Node* root;
    void traverseHelper(Node*);
public:
    GeneralTree();
    bool operator==(const GeneralTree&) const;
    Node* getRoot() const;
    void setRoot(Node*);
    File getRootData() const;
    void setRootData(const File&);
    std::vector<Node*> getChildren(Node*) const;
    void setParent(Node*, Node*);
    void insert(Node*, Node*);
    void insert(const File&, const File&);
    void remove(Node*);
    void traverse();
};

Node::Node(const File& f) : data{f} {}

Node::~Node() {
    for (Node* child : children) {
        delete child;
    }
}

Node* Node::getParent() const {
    return parent;
}

bool Node::operator==(const Node& other) const {
    return data.getName() == other.data.getName() && parent == other.parent && children == other.children;
}

void Node::addChild(Node* n) {
    children.push_back(n);
}

void Node::removeChild(Node* childNode) {
    auto it = std::find(children.begin(), children.end(), childNode);
    if (it != children.end()) {
        children.erase(it);
    }
}

GeneralTree::GeneralTree() : root(nullptr) {}

void GeneralTree::traverseHelper(Node* node) {
    if (node == nullptr)
        return;
    std::cout << node->data.getName() << " ";
    for (Node* child : node->children)
        traverseHelper(child);
}
    
bool GeneralTree::operator==(const GeneralTree& other) const {
    return root == nullptr && other.root == nullptr
           || (root != nullptr && other.root != nullptr && *root == *other.root);
}

Node* GeneralTree::getRoot() const {
    return root;
}

void GeneralTree::setRoot(Node* r) {
    root = r;
}

File GeneralTree::getRootData() const {
    return root->data;
}
void GeneralTree::setRootData(const File& d) {
    if (root == nullptr) {
        root = new Node(d);
    } else {
        root->data = d;
    }
}

std::vector<Node*> GeneralTree::getChildren(Node* node) const {
    return node->children;
}

void GeneralTree::setParent(Node* child, Node* parent) {
    child->parent = parent;
}

void GeneralTree::insert(Node* parentNode, Node* childNode) {
    childNode->parent = parentNode;
    parentNode->children.push_back(childNode);
}

void GeneralTree::insert(const File& parentData, const File& data) {
    Node* newNode = new Node(data);
    if (root == nullptr) {
        root = newNode;
        return;
    }
    std::vector<Node*> nodesQueue;
    nodesQueue.push_back(root);
    while (!nodesQueue.empty()) {
        Node* curr = nodesQueue.front();
        nodesQueue.erase(nodesQueue.begin());
        if (curr->data.getName() == parentData.getName()) {
            curr->children.push_back(newNode);
            return;
        }
        for (Node* child : curr->children)
            nodesQueue.push_back(child);
    }
    delete newNode;
    std::cout << "Parent node not found. Node " << data.getName() << " was not inserted." << std::endl;
}

void GeneralTree::remove(Node* node) {
    if (root == nullptr) {
        std::cout << "Tree is empty." << std::endl;
        return;
    }

    if (root == node) {
        delete root;
        root = nullptr;
        return;
    }

    std::vector<Node*> nodesQueue;
    nodesQueue.push_back(root);
    while (!nodesQueue.empty()) {
        Node* curr = nodesQueue.front();
        nodesQueue.erase(nodesQueue.begin());
        for (size_t i = 0; i < curr->children.size(); i++) {
            if (curr->children[i] == node) {
                Node* removedNode = curr->children[i];
                curr->removeChild(node);
                delete removedNode;
                return;
            }
            nodesQueue.push_back(curr->children[i]);
        }
    }
    std::cout << "Node not found. It was not removed." << std::endl;
}


void GeneralTree::traverse() {
    if (root == nullptr) {
        std::cout << "Tree is empty." << std::endl;
        return;
    }
    std::cout << "Tree traversal: ";
    traverseHelper(root);
    std::cout << std::endl;
}

} // namespace LinuxEmulator

#endif // LINUX_EMULATOR_GTREE_H