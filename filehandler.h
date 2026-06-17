#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Maximum rows and columns  
const int MAX_ROWS = 500;
const int MAX_COLS = 10;
//this struct will represent a row
// for example In students.txt one line = one student
struct CSVRow {
    string data[MAX_COLS];  //  every column value;
    int colCount;           //how many cols in a row; 
};

// to read data from files
void readTXT(string filename, CSVRow rows[], int &rowCount, int &colCount);

// (overwrite)
void writeTXT(string filename, CSVRow rows[], int rowCount, int colCount);

// to add one row in a file
void appendTXT(string filename, CSVRow row, int colCount);

// specifically find by(roll no) 
bool findRow(string filename, string value, int colIndex, CSVRow &result);

// Check existence;
bool rowExists(string filename, string value, int colIndex);

#endif