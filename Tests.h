#ifndef TESTTASK_TESTS_H
#define TESTTASK_TESTS_H

#include "test_runner.h"
#include "utilities.h"

#include <cstring>

void TestParsePath() {
    {
        try {
            char *argv[3] = {(char *) "..TestTask", (char *) "Tests/in.csv", (char *) "Tests/in.txt"};
            auto path = parsePath(3, argv);
            FAIL("Wrong number of arguments");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Wrong number of arguments") == 0);
        }
    }

    {
        try {
            char *argv[2] = {(char *) "..TestTask", (char *) "non-existent.csv"};
            auto path = parsePath(2, argv);
            FAIL("No such file: non-existent.csv");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "No such file: non-existent.csv") == 0);
        }
    }

    {
        try {
            char *argv[2] = {(char *) "..TestTask", (char *) "../Tests/in.txt"};
            auto path = parsePath(2, argv);
            FAIL("Received not a .csv file: Tests/in.txt");
        }
        catch (const invalid_argument &ex) {
            ASSERT(strcmp(ex.what(), "Received not a .csv file: ../Tests/in.txt") == 0);
        }
    }
}

#endif //TESTTASK_TESTS_H
