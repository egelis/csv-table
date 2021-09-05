#include "utilities.h"

#include <filesystem>

using namespace std;

string parsePath(int argc, char *argv[]) {
    if (argc != 2) {
        throw invalid_argument("Wrong number of arguments");
    }
    return argv[1];
}

void checkPath(const string &path) {
    if (!filesystem::exists(path)) {
        throw invalid_argument("No such file: " + path);
    }
    if (!path.ends_with(".csv")) {
        throw invalid_argument("Received not a .csv file: " + path);
    }
}

void deleteSpaces(string &str) {
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
}

bool isInteger(const string &str) {
    if (str.empty()) {
        return false;
    }

    if (str[0] != '-' && !isdigit(str[0])) {
        return false;
    }
    for (int i = 1; i < str.size(); ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

map<int, string> reverseMap(const map<string, int> &m) {
    map<int, string> reversed;
    for (const auto &[key, value]: m) {
        reversed[value] = key;
    }
    return reversed;
}

int applyOperator(char op, int left, int right) {
    int result = 0;
    switch (op) {
        case '+':
            result = left + right;
            break;
        case '-':
            result = left - right;
            break;
        case '*':
            result = left * right;
            break;
        case '/':
            if (right == 0) {
                throw std::invalid_argument("Division by 0");
            }
            result = left / right;
            break;
        default:
            throw invalid_argument("Invalid operator in expression");
    }
    return result;
}
