#include "utilities.h"

#include <filesystem>

std::string parsePath(int argc, char *argv[]) {
    if (argc != 2) {
        throw std::invalid_argument("Wrong number of arguments");
    }
    return argv[1];
}

void checkPath(const std::string &path) {
    if (!std::filesystem::exists(path)) {
        throw std::invalid_argument("No such file: " + path);
    }
    if (!path.ends_with(".csv")) {
        throw std::invalid_argument("Received not a .csv file: " + path);
    }
}

void deleteSpaces(std::string &str) {
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
}