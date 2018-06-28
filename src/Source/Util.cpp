//
// Created by oops on 18-6-3.
//
#include <iostream>
#include <regex>
#include "../Header/Util.h"

string replaceSingleSlashToDoubleSlahes(const string &strWithSingleSlash) {
    //将单斜杠路径(/)同一替换成双斜杠(//)路径分隔符
    string mTempStr = strWithSingleSlash;
    for (unsigned int i = 0; i < mTempStr.length(); i++) {
        if (mTempStr[i] == '\\') {
            mTempStr.replace(i, 1, "\\\\");
            ++i;
        }
    }
    return mTempStr;
}

/**
 * 判断是否以预处理命令开头
 * @param preCmd 预处理命令
 * @param line 要判断的代码
 * @return 1代表字符串line以预处理命令cmd开头
 */
bool startWith(string preCmd, string line) {
    if (preCmd == "#") {
        regex regex2("^" + preCmd);
        cout << regex_search(line, regex2);
        return regex_search(line, regex2);
    }
    string n_line = trim(line);
    string pattern("^\\b" + preCmd + "\\b");
    regex regex1(pattern);
    cout << regex_search(line, regex1);
    return regex_search(line, regex1);
}

/**
 * 去除首尾空格
 * @param s 字符串
 * @return 去除首尾空格后的字符串
 */
string trim(string &s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

/**
 * 以任意字符集合切割字符串
 * @param s 待切割字符串
 * @param v 切割后的子串集合
 * @param c 分隔符集合
 */
void splitString(string &s, vector<string> &v, string &c) {   //将字符串按指定字符集分割
    string::size_type pos1, pos2;
    pos2 = s.find_first_of(c);
    pos1 = 0;
    while (string::npos != pos2) {
        if ((pos2 - pos1) != 0u)
            v.push_back(s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = s.find_first_of(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

