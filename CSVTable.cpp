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

    int count = 0;
    for (size_t row = 0; row < rows_size; row++) {
        for (size_t col = 0; col < cols_size; col++) {
            if (!visited[cols_size * row + col]) {
                count++;
                evaluateCell(row, col);
            }
        }
    }
    cout << "Count: " << count << endl;
}

void CSVTable::evaluateCell(size_t row, size_t col) {
    if (isInteger(table[row][col])) {
        addToEvaluatedAndSetVisited(row, col, stoi(table[row][col]));
    }
    else {
        set<string> empty_cell_stack;
        evaluateExpr(row, col);
    }
}

void CSVTable::addToEvaluatedAndSetVisited(size_t row, size_t col, int value) {
    evaluated_table[cols_size * row + col] = value;
    visited[cols_size * row + col] = true;
}

int CSVTable::evaluateExpr(size_t row, size_t col) {
    const string &cell = table[row][col];

    auto[l_operand, operation, r_operand] = parseExpr(cell);

    int left  = isInteger(l_operand) ? stoi(l_operand) : evaluateRef(l_operand);
    int right = isInteger(r_operand) ? stoi(r_operand) : evaluateRef(r_operand);

    int result;
    switch (operation) {
        case '+':
            result = left + right;
            break;
        case '-':
            result = left - right;
            break;
        case '*':
            result = left * right;
            break;
        case '/':
            if (right == 0) {
                throw std::invalid_argument("Division by 0");
            }
            result = left / right;
            break;
        default:
            throw invalid_argument("Invalid operator in expression");
    }

    addToEvaluatedAndSetVisited(row, col, result);
    return result;
}

int CSVTable::evaluateRef(const string &ref) {
    checkRef(ref);

    size_t pos = ref.find_first_of("123456789");
    string col = ref.substr(0, pos);
    string row = ref.substr(pos);

    if (!col_to_index.count(col) || !row_to_index.count(row)) {
        throw invalid_argument("Invalid cell format, reference to a non-existent cell");
    }

    auto col_ind = col_to_index[col];
    auto row_ind = row_to_index[row];

    if (isInteger(table[row_ind][col_ind])) {
        int number = stoi(table[row_ind][col_ind]);
        addToEvaluatedAndSetVisited(row_ind, col_ind,number);
        return number;
    } else {
        return evaluateExpr(row_to_index[row], col_to_index[col]);
    }
}

void CSVTable::checkRef(const std::string &ref) {
    if (ref.empty()) {
        throw invalid_argument("Invalid reference to a cell");
    }

    size_t pos;
    for (pos = 0; pos < ref.size(); ++pos) {
        if (isdigit(ref[pos]))
            break;
    }

    for (;pos < ref.size(); ++pos) {
        if (!isdigit(ref[pos])) {
            throw invalid_argument("Invalid reference to a cell");
        }
    }
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
