#ifndef TESTTASK_UTILITIES_H
#define TESTTASK_UTILITIES_H

#include <filesystem>
#include <exception>

string parsePath(int argc, char *argv[]) {
    if (argc != 2) {
        throw invalid_argument("Wrong number of arguments");
    }

    return argv[1];
}

#endif //TESTTASK_UTILITIES_H
