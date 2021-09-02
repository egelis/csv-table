#include <iostream>
#include <vector>
#include <filesystem>

#include "CSVTable.h"

using namespace std;

/*
 Структура CSVTable:
 map<string, string>

*/
string parsePath(int argc, char *argv[]) {
    string path;
    try {
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
    catch (const exception &ex) {
        cerr << ex.what() << '\n';
        exit(1);
    }
}


int main(int argc, char *argv[]) {
    string path = parsePath(argc, argv);

    CSVTable table(path);
    auto res = table.getData();

    for (auto &row: res) {
        for (auto &col: row) {
            cout << col << ' ';
        }
        cout << endl;
    }

    return 0;
}
