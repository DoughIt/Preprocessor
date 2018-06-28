//
// Created by oops on 18-6-3.
//

#ifndef LAB3_PREPROCESSOR_H
#define LAB3_PREPROCESSOR_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include "Node.h"
#include <stack>
using namespace std;

class Preprocessor {
private:
    string preCode;
    string newCode;
    string sign;
    map<string, Node> defines;
    stack<bool> statuses;

    void handle_common(string line);

    string replace_define(const string &line);

    void handle_define(string line);

    void handle_undef(string line);

    void handle_if(string line);

    void handle_ifdef(string line);

    void handle_ifndef(string line);

    void handle_else();

    void handle_elif(string line);

    void handle_endif(string line);

    void handle_include(string line);

    bool has_customized_include_file(const string &filePath);

    void start(const string &inputFile);

    void handles(const string &line);

    int which_pre_cmd(const string &line);

public:
    explicit Preprocessor(string code);

    virtual ~Preprocessor();

    string preprocessor();

};


#endif //LAB3_PREPROCESSOR_H
