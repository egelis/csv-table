#include "CSVTable.h"

#include <iostream>
#include <filesystem>


CSVTable::CSVTable(const string &path) {
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
        data.emplace_back(parseNextRow(line));
    }
}

void CSVTable::parseHeader(const string &header) {
    stringstream ss(header);
    string cell;

    getline(ss, cell, ','); // skipping empty cell
    deleteSpaces(cell);
    checkFirstEmptyCell(cell);

    for (int i = 0; getline(ss, cell, ','); i++) {
        deleteSpaces(cell);
        checkHeaderCell(cell);
        col_to_index[cell] = i;
    }
}

void CSVTable::deleteSpaces(string &str) {
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
}

void CSVTable::checkHeaderCell(const string &cell) {
    checkEmptyColumnName(cell);
    checkRepeatedColName(cell);
    checkNumInColName(cell);
}

void CSVTable::checkFirstEmptyCell(const string &cell) {
    if (!cell.empty()) {
        throw invalid_argument("Incorrect table format, first cell must be empty");
    }
}

void CSVTable::checkRepeatedColName(const string &cell) {
    if (col_to_index.count(cell)) {
        throw invalid_argument("Table has repeated cells in header");
    }
}

void CSVTable::checkNumInColName(const string &cell) {
    if (std::any_of(cell.begin(), cell.end(), ::isdigit)) {
        throw invalid_argument("Incorrect table format, there can be no numbers in the column name");
    }
}

void CSVTable::checkEmptyColumnName(const string &cell) {
    if (cell.empty()) {
        throw invalid_argument("Incorrect table format, has empty column name");
    }
}

vector<string> CSVTable::parseNextRow(const string &line) {
    vector<string> row;
    stringstream ss(line);

    getRowNum(ss);

    string cell;
    while (getline(ss, cell, ',')) {
        deleteSpaces(cell);
        checkEmptyCell(cell);
        row.emplace_back(cell);
    }

    checkRowSize(row.size());

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
    checkEmptyRowNum(cell);
    checkLetterInRowName(cell);
    checkPositiveNum(cell);
    checkRepeatedRowName(cell);
}

void CSVTable::checkEmptyRowNum(const string &cell) {
    if (cell.empty()) {
        throw invalid_argument("Incorrect table format, has empty row number");
    }
}

void CSVTable::checkLetterInRowName(const string &cell) {
    if (std::any_of(cell.begin(), cell.end(), ::isalpha)) {
        throw invalid_argument("Incorrect table format, there can be no letters in the row name");
    }
}

void CSVTable::checkPositiveNum(const string &cell) {
    if (stoi(cell) < 1) {
        throw invalid_argument("Incorrect table format, row number isn't positive");
    }
}

void CSVTable::checkRepeatedRowName(const string &cell) {
    if (row_to_index.count(cell)) {
        throw invalid_argument("Incorrect table format, table has repeated rows names");
    }
}

void CSVTable::checkEmptyCell(const string &cell) {
    if (cell.empty()) {
        throw invalid_argument("Incorrect table format, has empty cell");
    }
}

void CSVTable::checkRowSize(size_t row_size) {
    if (row_size != col_to_index.size()) {
        throw invalid_argument("Incorrect table format, wrong row size");
    }
}

vector<vector<string>> CSVTable::getData() {
    return data;
}
