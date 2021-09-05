#include <iostream>

#include "utilities.h"
#include "CSVTable.h"

using namespace std;

int main(int argc, char *argv[]) {
    try {
        string path = parsePath(argc, argv);

        CSVTable table(path);
        table.evaluateTable();
        table.printEvaluated(cout);
    }
    catch (const invalid_argument &ex) {
        cerr << ex.what() << endl;
        return 1;
    }

    return 0;
}
