#include "filehandler.h"

void readTXT(string filename, CSVRow rows[], int &rowCount, int &colCount) {
    ifstream file(filename.c_str());
    rowCount = 0;
    colCount = 0;

    if (!file.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        return;
    }
    string header;
    getline(file, header);
    string line;
    while (getline(file, line) && rowCount < MAX_ROWS) {
        CSVRow currentRow;
        currentRow.colCount = 0;
        string cell = "";
        bool inQuotes = false;

        //  check every character
        for (int i = 0; i < line.length(); i++) {
            char c = line[i];

            if (c == '"') {

                inQuotes = !inQuotes;
            }
            else if (c == ',' && !inQuotes) {

                currentRow.data[currentRow.colCount] = cell;
                currentRow.colCount++;
                cell = "";
            }
            else {
                // Normal character - add in  cell
                cell += c;
            }
        }

        //  add last cell;
        currentRow.data[currentRow.colCount] = cell;
        currentRow.colCount++;

        // store in array
        rows[rowCount] = currentRow;
        rowCount++;

        // set column count from first row data
        if (rowCount == 1) colCount = currentRow.colCount;
    }

    file.close();
}

void writeTXT(string filename, CSVRow rows[], int rowCount, int colCount) {
    ofstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "Error: Could not write " << filename << endl;
        return;
    }
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < rows[i].colCount; j++) {

            if (rows[i].data[j].find(',') != string::npos) {
                file << "\"" << rows[i].data[j] << "\"";
            } else {
                file << rows[i].data[j];
            }

            // Last column ke baad comma nahi
            if (j < rows[i].colCount - 1) file << ",";
        }
        file << endl;  // Next line
    }
    file.close();
}

// appendTXT: add row in a file
void appendTXT(string filename, CSVRow row, int colCount) {
    ofstream file(filename.c_str(), ios::app);  // append mode
    if (!file.is_open()) {
        cout << "Error: Could not append " << filename << endl;
        return;
    }

    for (int j = 0; j < row.colCount; j++) {
        if (row.data[j].find(',') != string::npos) {
            file << "\"" << row.data[j] << "\"";
        } else {
            file << row.data[j];
        }
        if (j < row.colCount - 1) file << ",";
    }
    file << endl;
    file.close();
}

// Example:  find student by their roll number
bool findRow(string filename, string value, int colIndex, CSVRow &result) {
    CSVRow rows[MAX_ROWS];
    int rowCount = 0, colCount = 0;
    readTXT(filename, rows, rowCount, colCount);


    for (int i = 0; i < rowCount; i++) {
        if (colIndex < rows[i].colCount && rows[i].data[colIndex] == value) {
            result = rows[i];
            return true;
        }
    }
    return false;
}

//  check rowExists
bool rowExists(string filename, string value, int colIndex) {
    CSVRow dummy;
    return findRow(filename, value, colIndex, dummy);
}
