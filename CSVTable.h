#ifndef TESTTASK_CSVTABLE_H
#define TESTTASK_CSVTABLE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class CSVTable {
public:
    explicit CSVTable (const string &path);

    vector<vector<string>> getData();

private:
    bool isCellsRepeated(const vector<string> &header);
    vector<string> parseHeader(const string &header);
    static vector<string> getNextParsedLine(const string &line);

    vector<vector<string>> data;
};


#endif //TESTTASK_CSVTABLE_H
