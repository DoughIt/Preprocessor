//
// Created by oops on 18-6-3.
//

#ifndef LAB3_UTIL_H
#define LAB3_UTIL_H

#include <string>
#include <vector>

using namespace std;

string replaceSingleSlashToDoubleSlahes(const string &strWithSingleSlash);

bool startWith(string preCmd, string line);

string trim(string &s);

void splitString(string &s, vector<string> &v, string &c);


#endif //LAB3_UTIL_H
