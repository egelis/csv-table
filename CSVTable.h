#ifndef TESTTASK_CSVTABLE_H
#define TESTTASK_CSVTABLE_H

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

using namespace std;

class CSVTable {
private:
    ifstream in;
    unordered_map<string, int> col_to_index;
    unordered_map<int, int> row_to_index;
    vector<vector<string>> data;

public:
    explicit CSVTable(const string &path);

    vector<vector<string>> getData();

private:
    static void checkPath(const string &path);
    void checkRepeatedCell(const string &cell);
    void checkEmptyCell(const string &cell);
    void parseHeader(const string &header);
    vector<string> getNextParsedLine(const string &line);

    void readCSV(const string &path);
};


#endif //TESTTASK_CSVTABLE_H
