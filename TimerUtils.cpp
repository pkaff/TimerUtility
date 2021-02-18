#include "TimerUtils.hpp"
#include <iostream>
#include <stack>
#include <sstream>
#include <fstream>

void Timer::startTimePoint(const std::string& identifier) {
    curNode = curNode->addChild(identifier);
}

void Timer::stopCurrTimePoint() {
    if (curNode->parent) {
        curNode->stopTime();
        curNode = curNode->parent;
    } else {
        std::cerr << "Tried to stop timer for function call, but timer was aleady at root level." << std::endl;
    }
}

void Timer::dump() {
    if (curNode->parent) {
        std::cerr << "Tried to dump timer info but cur node is not root." << std::endl;
    } else {
        std::stringstream logfile;
        std::stack<const Node*> s;
        s.push(curNode);
        while (!s.empty()) {
            const Node* node = s.top();
            s.pop();
            for (size_t d = 0; d < node->depth; ++d) {
                logfile << "  ";
            }
            logfile << node->tag << ";" << node->duration << std::endl;
            for (auto it = node->children.rbegin(); it != node->children.rend(); ++it) {
                s.push(*it);
            }
        }

        std::ofstream myfile;
        myfile.open("/tmp/timer.txt");
        myfile << logfile.rdbuf();
        myfile.close();
    }
}