#include "CSVTable.h"

#include <iostream>
#include <sstream>

CSVTable::CSVTable(const string &path) {
    checkPath(path);
    in.open(path);
    readCSV(path);
}

void CSVTable::readCSV(const string &path) {
    string line;

    getline(in, line);
    parseHeader(line);

    while (getline(in, line, '\n')) {
        data.emplace_back(parseNextRow(line));
    }
}

void CSVTable::parseHeader(const string &header) {
    stringstream ss(header);
    string cell;

    // skipping first empty cell
    getline(ss, cell, ',');
    deleteSpaces(cell);
    if (!cell.empty()) {
        throw invalid_argument("Incorrect table format, first cell must be empty");
    }

    for (int i = 0; getline(ss, cell, ','); i++) {
        deleteSpaces(cell);
        checkHeaderCell(cell);
        col_to_index[cell] = i;
    }
}

void CSVTable::checkHeaderCell(const string &cell) {
    if (cell.empty()) {
        throw invalid_argument("Incorrect table format, has empty column name");
    }
    if (col_to_index.count(cell)) {
        throw invalid_argument("Table has repeated cells in header");
    }
    if (std::any_of(cell.begin(), cell.end(), ::isdigit)) {
        throw invalid_argument("Incorrect table format, there can be no numbers in the column name");
    }
}

vector<string> CSVTable::parseNextRow(const string &line) {
    vector<string> row;
    stringstream ss(line);

    getRowNum(ss);

    string cell;
    while (getline(ss, cell, ',')) {
        deleteSpaces(cell);
        if (cell.empty()) {
            throw invalid_argument("Incorrect table format, has empty cell");
        }
        row.emplace_back(cell);
    }

    if (row.size() != col_to_index.size()) {
        throw invalid_argument("Incorrect table format, wrong row size");
    }

    return row;
}

void CSVTable::getRowNum(stringstream &ss) {
    string cell;

    static int index = 0;
    getline(ss, cell, ',');
    deleteSpaces(cell);
    checkRowNumCell(cell);

    row_to_index[cell] = index++;
}

void CSVTable::checkRowNumCell(const string &cell) {
    if (cell.empty()) {
        throw invalid_argument("Incorrect table format, has empty row number");
    }
    if (std::any_of(cell.begin(), cell.end(), ::isalpha)) {
        throw invalid_argument("Incorrect table format, there can be no letters in the row name");
    }
    if (stoi(cell) < 1) {
        throw invalid_argument("Incorrect table format, row number isn't positive");
    }
    if (row_to_index.count(cell)) {
        throw invalid_argument("Incorrect table format, table has repeated rows names");
    }
}

vector<vector<string>> CSVTable::getData() {
    return data;
}

void CSVTable::evaluateTable() {

}
