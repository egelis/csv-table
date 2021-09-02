#include <iostream>

#include "test_runner.h"
#include "Tests.h"

#include "utilities.h"
#include "CSVTable.h"

using namespace std;


int main(int argc, char *argv[]) {
    {
        TestRunner tr;
        RUN_TEST(tr, TestParsePath);
        RUN_TEST(tr, TestCSVTableGetPath);
        RUN_TEST(tr, TestCSVTableHeader);
        RUN_TEST(tr, TestSimpleTable);
        RUN_TEST(tr, TestRows);
    }

    try {
        string path = parsePath(argc, argv);

        CSVTable table(path);

        auto res = table.getData();
        for (auto &row: res) {
            for (auto &col: row) {
                cout << "'" << col << "'" << ' ';
            }
            cout << endl;
        }
    }
    catch (const invalid_argument &ex) {
        cerr << ex.what() << endl;
        return 1;
    }

    return 0;
}
