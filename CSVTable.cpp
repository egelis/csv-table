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

    for (int index = 0; getline(in, line, '\n'); ++index) {
        table.emplace_back(parseNextRow(line, index));
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

    for (int index = 0; getline(ss, cell, ','); index++) {
        deleteSpaces(cell);
        checkHeaderCell(cell);
        col_to_index[cell] = index;
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

vector<string> CSVTable::parseNextRow(const string &line, int index) {
    vector<string> row;
    stringstream ss(line);

    getRowNum(ss, index);

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

void CSVTable::getRowNum(stringstream &ss, int index) {
    string cell;

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

    auto left = isInteger(l_operand) ? stoi(l_operand) : parseRef(l_operand);
    auto right = isInteger(r_operand) ? stoi(r_operand) : parseRef(r_operand);

    switch (operation) {
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '*':
            return left * right;
        case '/':
            if (right == 0) {
                throw std::invalid_argument("Division by 0");
            }
            return left / right;
        default:
            throw invalid_argument("Invalid operator in expression");
    }

    // visited = true;
    // evaluated_table +
}

int CSVTable::parseRef(const string &ref) {
    checkRef(ref);

    size_t pos = ref.find_first_of("123456789");
    string col = ref.substr(0, pos);
    string row = ref.substr(pos);

    if (!col_to_index.count(col) || !row_to_index.count(row)) {
        throw invalid_argument("Invalid reference to the cell, 2");
    }

    set<string> empty_cell_stack;
    return evaluateExpr(table[row_to_index[row]][col_to_index[col]], empty_cell_stack);
}

tuple<string, char, string>
CSVTable::parseExpr(const string &cell) {
    if (cell[0] != '=') {
        throw invalid_argument("Invalid cell format, '=' expected in cell");
    }

    auto pos = cell.find_first_of("+-*/");
    if (pos == std::string::npos) {
        throw invalid_argument("Invalid cell format, can't find operation in expression");
    }
    char operation = cell[pos];

    pos = cell.find(operation);
    string left = cell.substr(1, pos - 1);
    string right = cell.substr(pos + 1);

    return make_tuple(left, operation, right);
}
