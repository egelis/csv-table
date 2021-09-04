#ifndef TESTTASK_UTILITIES_H
#define TESTTASK_UTILITIES_H

#include <string>

std::string parsePath(int argc, char *argv[]);

void checkPath(const std::string &path);

void deleteSpaces(std::string &str);

bool isInteger(const std::string &str);

#endif //TESTTASK_UTILITIES_H
