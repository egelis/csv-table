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

    parseRowNum(ss, index);

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

void CSVTable::parseRowNum(stringstream &ss, int index) {
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

void CSVTable::printEvaluated(ostream &os) {
    auto index_to_col = reverseMap(col_to_index);
    auto index_to_row = reverseMap(row_to_index);

    for (const auto &[index, col]: index_to_col) {
        os << "," << col;
    }
    os << endl;

    for (int row = 0; row < rows_size; row++) {
        os << index_to_row[row];
        for (int col = 0; col < cols_size; col++) {
            os << "," << evaluated_table[cols_size * row + col];
        }
        os << endl;
    }
}

void CSVTable::evaluateTable() {
    visited.resize(cols_size * rows_size, false);
    evaluated_table.resize(cols_size * rows_size);

    for (size_t row = 0; row < rows_size; row++) {
        for (size_t col = 0; col < cols_size; col++) {
            std::set<std::pair<size_t, size_t>> depended;
            if (!visited[cols_size * row + col]) {
                evaluateCell(row, col, depended);
            }
        }
    }
}

int CSVTable::evaluateCell(size_t row, size_t col, std::set<std::pair<size_t, size_t>> &depended) {
    // if call already evaluated
    if (visited[cols_size * row + col]) {
        return evaluated_table[cols_size * row + col];
    }
    // if call is just a number
    if (isInteger(table[row][col])) {
        int val = stoi(table[row][col]);
        addToEvaluatedAndSetVisited(row, col, val);
        return val;
    }

    auto[l_operand, operation, r_operand] = parseExpr(table[row][col]);

    auto unique_index = std::pair<size_t, size_t>{row, col};
    depended.insert(unique_index);

    int left  = evaluateOperand(l_operand, depended);
    int right = evaluateOperand(r_operand, depended);

    depended.erase(unique_index);

    int result = applyOperator(operation, left, right);

    addToEvaluatedAndSetVisited(row, col, result);
    return result;
}

void CSVTable::addToEvaluatedAndSetVisited(size_t row, size_t col, int value) {
    evaluated_table[cols_size * row + col] = value;
    visited[cols_size * row + col] = true;
}

int CSVTable::evaluateOperand(string &operand, set<pair<size_t, size_t>> &depended) {
    int result;
    if (isInteger(operand)){
        result = stoi(operand);
    } else {
        auto[row, col] = parseRef(operand);
        auto unique_index = std::pair{row, col};
        if (depended.contains(unique_index)) {
            throw invalid_argument("Invalid cell format, cycle dependencies");
        }
        result = evaluateCell(row, col, depended);
    }
    return result;
}

tuple<string, char, string>
CSVTable::parseExpr(const string &cell) {
    if (cell[0] != '=') {
        throw invalid_argument("Invalid cell format, '=' expected in cell");
    }

    int start_pos = 0;
    if (cell[1] == '-') {
        start_pos = 2;
    }

    auto operation_pos = cell.find_first_of("+-*/", start_pos);
    if (operation_pos == std::string::npos) {
        throw invalid_argument("Invalid cell format, can't find operation in expression");
    }

    char operation = cell[operation_pos];
    string left = cell.substr(1, operation_pos - 1);
    string right = cell.substr(operation_pos + 1);

    return make_tuple(left, operation, right);
}

tuple<int, int>
CSVTable::parseRef(const std::string &ref) {
    checkRef(ref);

    size_t pos = ref.find_first_of("123456789");
    string col = ref.substr(0, pos);
    string row = ref.substr(pos);

    if (!col_to_index.count(col) || !row_to_index.count(row)) {
        throw invalid_argument("Invalid cell format, reference to a non-existent cell");
    }

    return make_tuple(row_to_index[row], col_to_index[col]);
}

void CSVTable::checkRef(const std::string &ref) {
    if (ref.empty()) {
        throw invalid_argument("Invalid cell format, no operand found");
    }

    size_t pos;
    for (pos = 0; pos < ref.size(); ++pos) {
        if (isdigit(ref[pos]))
            break;
    }

    for (; pos < ref.size(); ++pos) {
        if (!isdigit(ref[pos])) {
            throw invalid_argument("Invalid reference to a cell");
        }
    }
}
