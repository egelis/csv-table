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
    unordered_map<string, int> row_to_index;
    vector<vector<string>> data;

public:
    explicit CSVTable(const string &path);

    vector<vector<string>> getData();

private:
    static void checkPath(const string &path);
    static void checkFirstEmptyCell(const string &cell);
    static void checkEmptyColumnName(const string &cell);
    static void checkEmptyRowNum(const string &cell);
    static void checkPositiveNum(const string &cell);
    static void checkEmptyCell(const string &cell);
    static void checkNumInColName(const string &cell);
    static void checkLetterInRowName(const string &cell);
    static void deleteSpaces(string &str);
    void checkRepeatedColName(const string &cell);
    void checkRepeatedRowName(const string &cell);
    void checkRowSize(size_t row_size);
    void checkHeaderCell(const string &cell);
    void checkRowNumCell(const string &cell);

    void readCSV(const string &path);
    void parseHeader(const string &header);
    vector<string> parseNextRow(const string &line);
    void getRowNum(stringstream &ss);
};


#endif //TESTTASK_CSVTABLE_H
