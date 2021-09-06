#include "test_runner.h"
#include "tests.h"

using namespace std;

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestParsePath);
    RUN_TEST(tr, TestIsInteger);
    RUN_TEST(tr, TestCSVTableGetPath);
    RUN_TEST(tr, TestCSVTableHeader);
    RUN_TEST(tr, TestCSVTableRows);
    RUN_TEST(tr, TestCSVTableExamples);
    RUN_TEST(tr, TestCSVTableCells);

    return 0;
}
