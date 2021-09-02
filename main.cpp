#include <iostream>

#include "test_runner.h"
#include "Tests.h"

#include "utilities.h"
#include "CSVTable.h"

using namespace std;


int main(int argc, char *argv[]) {
    TestRunner tr;
    RUN_TEST(tr, TestParsePath);

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
