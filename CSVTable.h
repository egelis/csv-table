#ifndef TESTTASK_CSVTABLE_H
#define TESTTASK_CSVTABLE_H

#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>

#include "utilities.h"

using namespace std;

class CSVTable {
private:
    ifstream in;
    vector<bool> visited;
    map<string, int> col_to_index, row_to_index;
    size_t cols_size, rows_size;

    vector<vector<string>> table;
    vector<int> evaluated_table;

public:
    explicit CSVTable(const string &path);

    void evaluateTable();

    vector<vector<string>> getData();

    vector<int> getEvaluated();

    void printEvaluated();

private:
    void checkHeaderCell(const string &cell);
    void checkRowNumCell(const string &cell);

    void readCSV(const string &path);
    void parseHeader(const string &header);
    vector<string> parseNextRow(const string &line);
    void getRowNum(stringstream &ss);

    void evaluateCell(size_t row, size_t col);
    int evaluateExpr(const string &cell, std::set<string> &cell_stack);

    tuple<string, char, string> parseExpr(const string &cell);
};


#endif //TESTTASK_CSVTABLE_H
