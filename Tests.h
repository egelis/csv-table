#ifndef TESTTASK_TESTS_H
#define TESTTASK_TESTS_H

#include "test_runner.h"
#include "utilities.h"
#include "CSVTable.h"

#include <cstring>

void TestParsePath() {
    {
        try {
            char *argv[3] = {(char *) "..TestTask", (char *) "Tests/in.csv", (char *) "Tests/in.txt"};
            auto path = parsePath(3, argv);
            FAIL("'Wrong number of arguments'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Wrong number of arguments") == 0);
        }
    }

    {
        char *argv[2] = {(char *) "..TestTask", (char *) "../Tests/in.csv"};
        auto path = parsePath(2, argv);
        ASSERT_EQUAL(path, "../Tests/in.csv");
    }
}

void TestCSVTableGetPath() {
    {
        try {
            CSVTable table("non-existent.csv");
            FAIL("'No such file: non-existent.csv'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "No such file: non-existent.csv") == 0);
        }
    }

    {
        try {
            CSVTable table("../Tests/in.txt");
            FAIL("'Received not a .csv file: Tests/in.txt'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Received not a .csv file: ../Tests/in.txt") == 0);
        }
    }
}

void TestCSVTableHeader() {
    {
        try {
            CSVTable table("../Tests/header1.csv");
            FAIL("'Table has repeated cells in header'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Table has repeated cells in header") == 0);
        }
    }

    {
        try {
            CSVTable table("../Tests/header2.csv");
            FAIL("'Table has repeated cells in header'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Table has repeated cells in header") == 0);
        }
    }

    {
        try {
            CSVTable table("../Tests/header3.csv");
            FAIL("'Incorrect table format'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format") == 0);
        }
    }
}

void TestSimpleTable() {
    {
        vector<vector<string>> expected = {{"1", "0",  "-1"},
                                           {"2", "6",  "0"},
                                           {"0", "19", "5"}};
        CSVTable table("../Tests/simple.csv");
        ASSERT_EQUAL(table.getData(), expected);
    }
}

#endif //TESTTASK_TESTS_H
