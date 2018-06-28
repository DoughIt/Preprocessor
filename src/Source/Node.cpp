//
// Created by oops on 18-6-3.
//
#include <string>
#include "../Header/Node.h"

Node::Node(string key, string value, string arg, bool valid) {
    this->key = move(key);
    this->value = move(value);
    this->arg = move(arg);
    this->valid = valid;
}

string Node::getKey() {
    return this->key;
}

string Node::getValue() {
    return this->value;
}

string Node::getArg() {
    return this->arg;
}

bool Node::isValid() {
    return this->valid;
}

void Node::setValid(bool valid) {
    this->valid = valid;
}

