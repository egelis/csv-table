#include "CSVTable.h"

#include <iostream>
#include <sstream>
#include <set>

CSVTable::CSVTable(const string &path) {
    checkPath(path);
    in.open(path);
    readCSV(path);

    cols_size = col_to_index.size();
    rows_size = row_to_index.size();
}

void CSVTable::readCSV(const string &path) {
    string line;

    getline(in, line);
    parseHeader(line);

    while (getline(in, line, '\n')) {
        table.emplace_back(parseNextRow(line));
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
    return table;
}

vector<int> CSVTable::getEvaluated() {
    return evaluated_table;
}

void CSVTable::printEvaluated() {
    for (size_t row = 0; row < rows_size; row++) {
        for (size_t col = 0; col < cols_size; col++) {
            cout << evaluated_table[cols_size * row + col] << ' ';
        }
        cout << endl;
    }
}

/*
 * Считываем ячейку:
 * - Если ячейка - число, то проверяем 'целое ли оно'
 * - Если это выражение, то вызываем evaluateCell(cell), где проверяем на 'соответствие виду =Arg1 OP Arg2',
 * 'если аргументы являются числами, то они целые', 'если есть ссылки, то они валидные'
 */

void CSVTable::evaluateTable() {
    visited.resize(cols_size * rows_size, false);
    evaluated_table.resize(cols_size * rows_size);

    for (size_t row = 0; row < rows_size; row++) {
        for (size_t col = 0; col < cols_size; col++) {
            if (!visited[cols_size * row + col]) {
                evaluateCell(row, col);
            }
        }
    }
}

void CSVTable::evaluateCell(size_t row, size_t col) {
    if (isInteger(table[row][col])) { // проверка на целое
        evaluated_table[cols_size * row + col] = (stoi(table[row][col]));
        visited[cols_size * row + col] = true;
    }
    else { // проверка на соответствие виду, на целые числа в операндах, на валидность ссылок в операндах
        set<string> empty_cell_stack;
        evaluated_table[cols_size * row + col] = evaluateExpr(table[row][col], empty_cell_stack);
    }
}

int CSVTable::evaluateExpr(const string &cell, set<string> &cell_stack) {
    if (isInteger(cell)) {
        return stoi(cell);
    }

    auto[l_operand, operation, r_operand] = parseExpr(cell);
    // TODO:: проверять операнды на числа, если они не числа, то пытаться спарсить ссылку
}

tuple<string, char, string>
CSVTable::parseExpr(const string &cell) {
    if (cell[0] != '=') {
        throw invalid_argument("Invalid cell format, can't meet '=' in cell");
    }

    auto pos = cell.find_first_of("+-*/");
    if (pos == std::string::npos) {
        throw invalid_argument("Invalid cell format, can't find operation in cell");
    }
    char operation = cell[pos];

    pos = cell.find(operation);
    string left = cell.substr(1, pos - 1);
    string right = cell.substr(pos + 1);

    return make_tuple(left, operation, right);
}

/*
bool isExpression(const std::string &str) {
    if (!str.empty()) {
        if (str[0] != '=') {
            return false;
        }

        for (int i = 0; i < str.size(); ++i) {

        }

        return true;
    }
    return false;
}
*/

/*
switch (operation) {
            case '+':
                return evaluateExpr(left, cell_stack) + evaluateExpr(right, cell_stack);
            case '-':
                return evaluateExpr(left, cell_stack) - evaluateExpr(right, cell_stack);
            case '/':
                if (right == "0") {
                    throw std::logic_error("Can't divide by 0");
                }
                return evaluateExpr(left, cell_stack) / evaluateExpr(right, cell_stack);
            case '*':
                return evaluateExpr(left, cell_stack) * evaluateExpr(right, cell_stack);
        }
*/