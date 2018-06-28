//
// Created by oops on 18-6-3.
//
#include <regex>
#include <iostream>
#include "../Header/Node.h"
#include "../Header/Util.h"
#include "../Header/Preprocessor.h"


string preCmds[9] = {//预处理命令
        "define",  //1
        "undef",   //2
        "if",      //3
        "ifdef",   //4
        "ifndef",  //5
        "else",    //6
        "elif",    //7
        "endif",   //8
        "include"  //9
};

/**
 * 普通代码
 */

void Preprocessor::handle_common(string line) {
    newCode.append(replace_define(line)).push_back('\n');
}

/**
 * 宏替换
 * @param line
 * @return
 */
string Preprocessor::replace_define(const string &line) {
    if (!statuses.top())
        return "";
    string tmp = line;
    for (auto it = defines.end(); it != defines.begin();) {
        it--;
        Node node = it->second;
        if (!node.getArg().empty()) {
            string key = node.getKey();
            unsigned long pos1;
            if ((pos1 = tmp.find(key)) != string::npos) {
                unsigned long pos2 = tmp.find_first_of("(", pos1);
                unsigned long pos3 = tmp.find_first_of(")", pos2);
                string t = tmp.substr(pos2, pos3);
                string arg = tmp.substr(pos2 + 1, pos3 - pos2 - 1);
                string tmpArg = node.getArg();
                string value = node.getValue();
                regex regex1("\"#" + tmpArg);
                value = regex_replace(value, regex1, arg + "\"");
                regex regex2(tmpArg);
                value = regex_replace(value, regex2, arg);
                string pattern(key + "(" + arg + ")");
                unsigned long index = tmp.find(key);
                unsigned long index2 = tmp.find(")", index);
                tmp.replace(index, index2 - index + 1, "");
                tmp.insert(index, value);
            }
        } else {
            string pattern{"\\b" + it->first + "\\b"};
            regex re(pattern);
            tmp = regex_replace(tmp, re, node.getValue());
            if (defines.count(node.getValue()) > 0) {
                regex regex1("\\b" + node.getValue() + "\\b");
                tmp = regex_replace(tmp, regex1, defines.find(node.getValue())->second.getValue());
            }
        }

    }
    return tmp;
}

/**
 * 处理宏替换 #define
 */
void Preprocessor::handle_define(string line) {
//    line = replace_define(line);
    if (!statuses.top()) {
        return;
    }
    vector<string> tokens1, tokens2;
    string sign1(" \t\n\r\f#()"), sign2(" \t\n\r\f#");
    splitString(line, tokens1, sign1);
    splitString(line, tokens2, sign2);
    if (tokens1[1] != tokens2[1]) {//函数
        unsigned long index = line.find_first_of(")");
        string value = line.substr(line.find_first_of(")") + 1);
        Node node(tokens1[1], trim(value), trim(tokens1[2]), true);
        defines.insert(make_pair(tokens2[1], node));
    } else {
        string value = line.substr(line.find_first_of(tokens2[1]) + tokens2[1].length());
        Node node(tokens2[1], trim(value), "", true);
        defines.insert(make_pair(tokens2[1], node));
    }
}

/**
* 处理宏替换 #undef
*/
void Preprocessor::handle_undef(string line) {
    vector<string> tokens;
    splitString(line, tokens, sign);
    defines.erase(tokens[1]);
//    defines[tokens[1]].setValid(0);
}

/**
 * 处理条件条件编译 #if
 */
void Preprocessor::handle_if(string line) {
    vector<string> tokens;
    string n_line = replace_define(line);
    splitString(n_line, tokens, sign);
    statuses.push("1" == tokens[1]);
}

/**
 * 处理条件条件编译 #ifdef
 */
void Preprocessor::handle_ifdef(string line) {
    vector<string> tokens;
    splitString(line, tokens, sign);
    statuses.push(defines.count(tokens[1]) > 0);
}

/**
 * 处理条件条件编译 #ifndef
 */
void Preprocessor::handle_ifndef(string line) {
    vector<string> tokens;
    splitString(line, tokens, sign);
    statuses.push(defines.count(tokens[1]) <= 0);
}

/**
 * 处理条件条件编译 #else
 */
void Preprocessor::handle_else() {
    bool status = statuses.top();
    statuses.pop();
    statuses.push(!status);
}

/**
 * 处理条件条件编译 #elif
 */
void Preprocessor::handle_elif(string line) {
    if (!statuses.top()) {
        statuses.pop();
        vector<string> tokens;
        string n_line = replace_define(line);
        splitString(n_line, tokens, sign);
        statuses.push(tokens[1] == "1");
    }
}

/**
 * 处理条件条件编译 #endif
 */
void Preprocessor::handle_endif(string line) {
    statuses.pop();
}

/**
 * 处理源文件包含 #include
 */
void Preprocessor::handle_include(string line) {
    //处理预编译命令#include
    string includeFilePath;//保存遍历时得到的#include文件名
    //判断是标准库还是自定义库
    if (line.find("<") != string::npos) {//若是标准库文件则不管
        newCode.append(line).push_back('\n');
        return;
    }
    //若是自定义库，则继续
    unsigned long mFirstPosition = line.find_first_of('"');
    unsigned long mSecondPosition = line.find_first_of('"', mFirstPosition + 1);
    includeFilePath = line.substr(mFirstPosition + 1, mSecondPosition - mFirstPosition - 1);
    if (includeFilePath.find(".h") != string::npos) {
        start(includeFilePath);
    } else {
        newCode.append(line).push_back('\n');
    }

}


/**
 * 判断是否包含其他库文件
 * @param filePath 需要判断的文件路径
 * @return
 */
bool Preprocessor::has_customized_include_file(const string &filePath) {
    ifstream in(filePath);
    if (!in.is_open()) {
        cout << "Broken input file." << endl;
        return false;
    }
    string line;
    while (getline(in, line)) {
        if (which_pre_cmd(line) == 9)//#include
            return true;
    }
    return false;
}

/**
 * 递归处理#include的自定义库文件
 * @param inputFile
 */
void Preprocessor::start(const string &inputFile) {
    //递归遍历#include的所有文件
    if (!has_customized_include_file(inputFile)) {
        //到达递归的base case，不包含其它自定义头文件
        ifstream innerInputFile("test/" + inputFile);
        string line;
        string tempPath = replaceSingleSlashToDoubleSlahes(inputFile);
        while (getline(innerInputFile, line)) {
            handles(line);
        }
    } else if (has_customized_include_file(inputFile)) {
        ifstream in(inputFile);
        string file;
        if (!in.is_open()) {
            cout << "Broken input file." << endl;
        } else {
            string line;
            while (getline(in, line)) {
                handles(line);
            }
            in.close();
        }

    } else {
        cout << "special case" << endl;
    }
}


/**
 * 判断属于哪一条预处理语句
 * @param line
 * @return 0表示不是预处理语句，否则返回对应预处理命令数字
 */
int Preprocessor::which_pre_cmd(const string &line) {
    int i = 0;
    for (const auto &cmd: preCmds) {
        i++;
        if (startWith("#", line) && startWith(cmd, line.substr(line.find_first_of("#") + 1))) {
            return i;
        }
    }
    return 0;
}

/**
 * 判断该执行哪个预处理操作
 * @param line
 */
void Preprocessor::handles(const string &line) {
    int type = which_pre_cmd(line);
    switch (type) {
        case 0:
            handle_common(line);
            break;
        case 1:
            handle_define(line);
            break;
        case 2:
            handle_undef(line);
            break;
        case 3:
            handle_if(line);
            break;
        case 4:
            handle_ifdef(line);
            break;
        case 5:
            handle_ifndef(line);
            break;
        case 6:
            handle_else();
            break;
        case 7:
            handle_elif(line);
            break;
        case 8:
            handle_endif(line);
            break;
        case 9:
            handle_include(line);
            break;
        default:
            break;
    }
}

string Preprocessor::preprocessor() {
    vector<string> lines;
    string sign = "\n";
    splitString(preCode, lines, sign); //将源文件切割成行
    for (auto line :lines) {
        handles(line);
    }
    return newCode;
}


/**
 * 构造方法
 * @param code 待预处理的代码
 */
Preprocessor::Preprocessor(string code) {
    this->preCode = move(code);
    this->newCode = "";
    this->sign = " \t\n\r\f#";
    this->statuses.push(true);

}


/**
 * 析构函数
 */
Preprocessor::~Preprocessor() = default;