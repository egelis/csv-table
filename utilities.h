#ifndef TESTTASK_UTILITIES_H
#define TESTTASK_UTILITIES_H

#include <filesystem>
#include <exception>

string parsePath(int argc, char *argv[]) {
    string path;

    if (argc != 2) {
        throw invalid_argument("Wrong number of arguments");
    }

    path = argv[1];

    if (!filesystem::exists(path)) {
        throw invalid_argument("No such file: " + path);
    }

    if (!path.ends_with(".csv")) {
        throw invalid_argument("Received not a .csv file: " + path);
    }

    return path;
}

#endif //TESTTASK_UTILITIES_H
