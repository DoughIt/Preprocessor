//
// Created by oops on 18-6-3.
//

#ifndef LAB3_NODE_H
#define LAB3_NODE_H


#include <string>

using namespace std;

class Node {
private:
    string key, value, arg;
    bool valid;
public:

    Node(string key, string value, string arg, bool valid);

    string getKey();

    string getValue();

    string getArg();

    bool isValid();

    void setValid(bool valid);
};


#endif //LAB3_NODE_H
