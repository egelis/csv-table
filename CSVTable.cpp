#include "CSVTable.h"

#include <iostream>
#include <filesystem>


CSVTable::CSVTable(const string &path){
    checkPath(path);
    in.open(path);
    readCSV(path);
}

void CSVTable::checkPath(const string &path) {
    if (!filesystem::exists(path)) {
        throw invalid_argument("No such file: " + path);
    }
    if (!path.ends_with(".csv")) {
        throw invalid_argument("Received not a .csv file: " + path);
    }
}

void CSVTable::readCSV(const string &path) {
    string line;

    getline(in, line);
    parseHeader(line);

    while (getline(in, line, '\n')) {
        data.emplace_back(getNextParsedLine(line));
    }
}

void CSVTable::parseHeader(const string &header) {
    stringstream ss(header);
    string cell;

    // skip empty cell and check if it exists
    getline(ss, cell, ',');
    checkEmptyCell(cell);

    for (int i = 0; getline(ss, cell, ','); i++) {
        checkRepeatedCell(cell);
        col_to_index[cell] = i;
    }
}

void CSVTable::checkEmptyCell(const string &cell) {
    if (!cell.empty()) {
        throw invalid_argument("Incorrect table format");
    }
}

void CSVTable::checkRepeatedCell(const string &cell) {
    if (col_to_index.count(cell)) {
        throw invalid_argument("Table has repeated cells in header");
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
