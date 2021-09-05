#ifndef TESTTASK_TESTS_H
#define TESTTASK_TESTS_H

#include "test_runner.h"
#include "utilities.h"
#include "CSVTable.h"

#include <cstring>

void TestIsInteger() {
    ASSERT(isInteger("-15"));
    ASSERT(isInteger("15"));
    ASSERT(!isInteger("-15.3"));
    ASSERT(!isInteger("l15"));
    ASSERT(!isInteger("15l"));
    ASSERT(!isInteger("1l5"));
    ASSERT(!isInteger("1-5"));
    ASSERT(!isInteger("-l5"));
    ASSERT(!isInteger("+15"));
    ASSERT(!isInteger(""));
}

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
            FAIL("'Incorrect table format, first cell must be empty'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, first cell must be empty") == 0);
        }
    }

    {
        try {
            CSVTable table("../Tests/header4.csv");
            FAIL("'Incorrect table format, has empty column name'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, has empty column name") == 0);
        }
    }

    {
        try {
            CSVTable table("../Tests/header5.csv");
            FAIL("'Incorrect table format, there can be no numbers in the column name'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, there can be no numbers in the column name") == 0);
        }
    }
}

void TestRows() {
    {
        try {
            CSVTable table("../Tests/row1.csv");
            FAIL("'Incorrect table format, has empty row number'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, has empty row number") == 0);
        }
    }

    {
        try {
            CSVTable table("../Tests/row2.csv");
            FAIL("'Incorrect table format, row number isn't positive'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, row number isn't positive") == 0);
        }
    }

    {
        try {
            CSVTable table("../Tests/row3.csv");
            FAIL("'Incorrect table format, table has repeated rows names'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, table has repeated rows names") == 0);
        }
    }

    {
        try {
            CSVTable table("../Tests/row4.csv");
            FAIL("'Incorrect table format, has empty cell'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, has empty cell") == 0);
        }
    }

    {
        try {
            CSVTable table("../Tests/row5.csv");
            FAIL("'Incorrect table format, wrong row size'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, wrong row size") == 0);
        }
    }

    {
        try {
            CSVTable table("../Tests/row6.csv");
            FAIL("'Incorrect table format, there can be no letters in the row name'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, there can be no letters in the row name") == 0);
        }
    }
}

void TestSimpleTable() {
    {
        vector<int> expected = {1, 0, -1,
                                2, 6, 0,
                                0, 19, 5,
                                -15, 3, 99};

        CSVTable table("../Tests/simple.csv");
        table.evaluateTable();
        ASSERT_EQUAL(table.getEvaluated(), expected);
    }
}

void TestHardTable() {
    {
//        vector<string> expected = {"", "A", "B", "C", "D", "E", "F", "G", "Cell",
//                                   "1", "1", "0", "1", "2", "8", "1", "4", "9",
//                                   "2", "2", "26", "0", "9", "123", "9", "60", "90",
//                                   "3", "9", "28", "9", "10", "6", "23", "7", "1",
//                                   "4", "6", "8", "9", "1", "2", "7", "9", "0",
//                                   "5", "3", "6", "123", "3", "13", "90", "28", "6",
//                                   "30", "0", "1", "5", "7", "6", "9", "10", "25"};
        vector<int> expected = {1, 0, 1, 2, 8, 1, 4, 9,
                                2, 26, 0, 9, 123, 9, 60, 90,
                                9, 28, 9, 10, 6, 23, 7, 1,
                                6, 8, 9, 1, 2, 7, 9, 0,
                                3, 6, 123, 3, 13, 90, 28, 6,
                                0, 1, 5, 7, 6, 9, 10, 25};

        CSVTable table("../Tests/hard1.csv");
        table.evaluateTable();
        ASSERT_EQUAL(table.getEvaluated(), expected);
    }
}

#endif //TESTTASK_TESTS_H
