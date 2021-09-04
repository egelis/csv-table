#ifndef TESTTASK_CSVTABLE_H
#define TESTTASK_CSVTABLE_H

#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "utilities.h"

using namespace std;

class CSVTable {
private:
    ifstream in;
    map<string, int> col_to_index;
    map<string, int> row_to_index;
    vector<vector<string>> data;

public:
    explicit CSVTable(const string &path);

    void evaluateTable();

    vector<vector<string>> getData();

private:
    void checkHeaderCell(const string &cell);
    void checkRowNumCell(const string &cell);

    void readCSV(const string &path);
    void parseHeader(const string &header);
    vector<string> parseNextRow(const string &line);
    void getRowNum(stringstream &ss);
};


#endif //TESTTASK_CSVTABLE_H
