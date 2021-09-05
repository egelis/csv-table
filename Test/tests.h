#ifndef TESTTASK_TESTS_H
#define TESTTASK_TESTS_H

#include "test_runner.h"
#include "../utilities.h"
#include "../CSVTable.h"

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
            char *argv[3] = {(char *) "..TestTask", (char *) "Test/in.csv", (char *) "Test/in.txt"};
            auto path = parsePath(3, argv);
            FAIL("'Wrong number of arguments'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Wrong number of arguments") == 0);
        }
    }

    {
        char *argv[2] = {(char *) "..TestTask", (char *) "../tests/in.csv"};
        auto path = parsePath(2, argv);
        ASSERT_EQUAL(path, "../tests/in.csv");
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
            CSVTable table("../tests/in.txt");
            FAIL("'Received not a .csv file: Test/in.txt'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Received not a .csv file: ../tests/in.txt") == 0);
        }
    }
}

void TestCSVTableHeader() {
    {
        try {
            CSVTable table("../tests/header1.csv");
            FAIL("'Table has repeated cells in header'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Table has repeated cells in header") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/header2.csv");
            FAIL("'Table has repeated cells in header'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Table has repeated cells in header") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/header3.csv");
            FAIL("'Incorrect table format, first cell must be empty'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, first cell must be empty") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/header4.csv");
            FAIL("'Incorrect table format, has empty column name'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, has empty column name") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/header5.csv");
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
            CSVTable table("../tests/row1.csv");
            FAIL("'Incorrect table format, has empty row number'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, has empty row number") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/row2.csv");
            FAIL("'Incorrect table format, row number isn't positive'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, row number isn't positive") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/row3.csv");
            FAIL("'Incorrect table format, table has repeated rows names'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, table has repeated rows names") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/row4.csv");
            FAIL("'Incorrect table format, has empty cell'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, has empty cell") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/row5.csv");
            FAIL("'Incorrect table format, wrong row size'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, wrong row size") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/row6.csv");
            FAIL("'Incorrect table format, there can be no letters in the row name'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Incorrect table format, there can be no letters in the row name") == 0);
        }
    }
}

void TestSimpleTable() {
    {
        string expected = ",A,B,Cell\n"
                          "1,1,0,-1\n"
                          "2,2,6,0\n"
                          "30,0,19,5\n"
                          "15,-15,3,99\n";

        CSVTable table("../tests/simple.csv");
        table.evaluateTable();

        ostringstream oss;
        table.printEvaluated(oss);

        ASSERT_EQUAL(oss.str(), expected);
    }
}

void TestHardTable() {
    {
        string expected = ",A,B,C,D,E,F,G,Cell\n"
                          "1,1,0,1,2,8,1,4,9\n"
                          "2,2,26,0,9,123,9,60,90\n"
                          "3,9,28,9,10,6,23,7,1\n"
                          "4,6,8,9,1,2,7,9,0\n"
                          "5,3,6,123,3,13,90,28,6\n"
                          "30,0,1,5,7,6,9,10,25\n";

        CSVTable table("../tests/hard1.csv");
        table.evaluateTable();

        ostringstream oss;
        table.printEvaluated(oss);

        ASSERT_EQUAL(oss.str(), expected);
    }

    {
        string expected = ",A,B,C\n"
                          "1,5,5,5\n"
                          "30,5,5,5\n"
                          "2,5,5,5\n";

        CSVTable table("../tests/hard2.csv");
        table.evaluateTable();

        ostringstream oss;
        table.printEvaluated(oss);

        ASSERT_EQUAL(oss.str(), expected);
    }

    {
        try {
            CSVTable table("../tests/hard3.csv");
            table.evaluateTable();
            FAIL("'Invalid cell format, reference to a non-existent cell'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Invalid cell format, reference to a non-existent cell") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/hard4.csv");
            table.evaluateTable();
            FAIL("'Invalid cell format, '=' expected in cell'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Invalid cell format, '=' expected in cell") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/hard5.csv");
            table.evaluateTable();
            FAIL("'Invalid cell format, can't find operation in expression'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Invalid cell format, can't find operation in expression") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/hard6.csv");
            table.evaluateTable();
            FAIL("'Invalid cell format, no operand found'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Invalid cell format, no operand found") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/hard7.csv");
            table.evaluateTable();
            FAIL("'Invalid reference to a cell'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Invalid reference to a cell") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/hard8.csv");
            table.evaluateTable();
            FAIL("'Invalid reference to a cell'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Invalid reference to a cell") == 0);
        }
    }

    {
        try {
            CSVTable table("../tests/hard9.csv");
            table.evaluateTable();
            FAIL("'Division by 0'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Division by 0") == 0);
        }
    }

    {
        string expected = ",A,B,Cell\n"
                          "1,1,0,-1\n"
                          "2,2,6,2\n"
                          "30,0,1,5\n"
                          "5,2,1,5\n";

        CSVTable table("../tests/hard10.csv");
        table.evaluateTable();

        ostringstream oss;
        table.printEvaluated(oss);

        ASSERT_EQUAL(oss.str(), expected);
    }

    {
        try {
            CSVTable table("../tests/hard11.csv");
            table.evaluateTable();
            FAIL("'Invalid cell format, cycle dependencies'");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Invalid cell format, cycle dependencies") == 0);
        }
    }
}

#endif //TESTTASK_TESTS_H
