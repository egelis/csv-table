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

    void printEvaluated(ostream &os);

private:
    void checkHeaderCell(const string &cell);
    void checkRowNumCell(const string &cell);
    static void checkRef(const string &ref);

    void readCSV(const string &path);
    void addToEvaluatedAndSetVisited(size_t row, size_t col, int value);

    static tuple<string, char, string> parseExpr(const string &cell);
    tuple<int, int> parseRef(const string &ref);
    vector<string> parseNextRow(const string &line, int index);
    void parseRowNum(stringstream &ss, int index);
    void parseHeader(const string &header);

    int evaluateCell(size_t row, size_t col, set<pair<size_t, size_t>> &depended);
    int evaluateOperand(string &operand, set<pair<size_t, size_t>> &depended);
};


#endif //TESTTASK_CSVTABLE_H
