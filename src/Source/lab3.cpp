#include <iostream>
#include <fstream>
#include "../Header/Preprocessor.h"

using namespace std;

string get_unprocessed_code(int number);

void put_processed_code(int number, string code);

void run_test(int test_case_number);

/**
 * IDE: Clion
 * Run Configurations: Working directory指向当前项目的src/Source目录
 *
 * @bug 如果没有配置上述工作目录，会不能打开测试文件（相对路径）
 * @return
 */
int main() {
    for (int test_case_number = 1; test_case_number <= 2; test_case_number++) {
        run_test(test_case_number);
    }
    return 0;
}

void run_test(int test_case_number) {
    string raw_code = get_unprocessed_code(test_case_number);
    Preprocessor prep(raw_code);
    string processed_code = prep.preprocessor();
    put_processed_code(test_case_number, processed_code);
}

string get_unprocessed_code(int number) {
    string filename = "test/test" + to_string(number) + ".cpp";
    string file;
    ifstream is(filename);
    if (!is.is_open()) {
        cout << "Broken input file.";
    } else {
        string line;
        while (getline(is, line)) {
            file.append(line).push_back('\n');
        }
        is.close();
    }
    return file;
}

void put_processed_code(int number, string code) {
    string filename = "test/test" + to_string(number) + ".out.cpp";
    ofstream os(filename);
    if (!os.is_open()) {
        cout << "Broken output file.";
    } else {
        os << code;
        os.close();
    }
}