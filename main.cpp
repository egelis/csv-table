#include <iostream>
#include <regex>

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
        RUN_TEST(tr, TestIsInteger);
        cerr << endl;
    }

    try {
        string path = parsePath(argc, argv);

        CSVTable table(path);

        auto res1 = table.getData();
        for (auto &row: res1) {
            for (auto &col: row) {
                cout << col << ' ';
            }
            cout << endl;
        }
        cout << endl;

        table.evaluateTable();
        table.printEvaluated();
    }
    catch (const invalid_argument &ex) {
        cerr << ex.what() << endl;
        return 1;
    }

    /*parseRef("Cell30");
    parseRef("30Cell");
    parseRef("Ce30ll");*/

    return 0;
}
