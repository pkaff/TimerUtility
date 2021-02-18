#pragma once
#include <string>
#include <chrono>
#include <vector>
#include <iostream>

class Timer {
    struct Node {
        Node* parent = nullptr;
        std::vector<Node*> children;
        std::string tag;
        std::chrono::steady_clock::time_point start;
        double duration;
        size_t depth;
        Node(const std::string& tag) : tag(tag), start(std::chrono::steady_clock::now()), depth(0) {}
        Node(Node* parent, const std::string& tag) : parent(parent), tag(tag), start(std::chrono::steady_clock::now()), depth(0) {
            const Node* n = this;
            while (n->parent) {
                ++depth;
                n = n->parent;
            }
        }
        Node* addChild(const std::string& tag) {
            children.emplace_back(new Node(this, tag));
            return children.back();
        }
        void stopTime() {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1e6;
        }
        ~Node() {
            for (Node* c : children) {
                delete c;
            }
        }
    };
    Node root;
    Node* curNode;

public:
    Timer() : root("root"), curNode(&root) { }
    void startTimePoint(const std::string& identifier);
    void stopCurrTimePoint();
    void dump();
};

extern Timer timer;
