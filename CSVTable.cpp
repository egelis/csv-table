#include "CSVTable.h"

#include <iostream>

vector<string> CSVTable::parseHeader(const string &header) {
    vector<string> res;
    stringstream ss(header);
    string cell;

    // pass empty cell
    getline(ss, cell, ',');

    while (getline(ss, cell, ',')) {
        res.emplace_back(cell);
    }

    if (isCellsRepeated(res)) {
        cerr << "Header cells repeats\n";
        exit(1);
    }

    return res;
}

bool CSVTable::isCellsRepeated(const vector<string> &header) {
    std::vector<std::string> copied_vector = header;
    sort(copied_vector.begin(), copied_vector.end());
    auto last = std::unique(copied_vector.begin(), copied_vector.end());
    return !(last == copied_vector.end());
}

CSVTable::CSVTable(const string &path) {
    ifstream in(path);
    string line;

    getline(in, line);
    auto header = parseHeader(line);
    cout << "Header: " << endl;
    for (auto &cell: header) {
        cout << cell << ' ';
    }
    cout << "\nHeader end" << endl;

    while (getline(in, line, '\n')) {
        data.emplace_back(getNextParsedLine(line));
    }
}

vector<string> CSVTable::getNextParsedLine(const string &line) {
    vector<string> res;
    stringstream ss(line);

    for (string cell; getline(ss, cell, ',');) {
        res.emplace_back(cell);
    }

    return res;
}

vector<vector<string>> CSVTable::getData() {
    return data;
}
