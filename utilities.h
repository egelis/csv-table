#ifndef TESTTASK_UTILITIES_H
#define TESTTASK_UTILITIES_H

#include <string>
#include <map>

std::string parsePath(int argc, char *argv[]);

void checkPath(const std::string &path);

void deleteSpaces(std::string &str);

bool isInteger(const std::string &str);

std::map<int, std::string> reverseMap(const std::map<std::string, int> &m);

int applyOperator(char op, int left, int right);

#endif //TESTTASK_UTILITIES_H
