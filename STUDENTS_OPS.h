#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include <iostream>
#include <string>
#include "filehandler.h"
using namespace std;

// to store Student data
struct Student {
    string roll_no;
    string name;
    string department;
    int semester;
    double cgpa;
    string status;
};

// check Roll number format (BSAI-YY-XXX)
bool isValidRollFormat(string roll);

// hve digits in name or not
bool hasDigits(string name);

// add new student
void addStudent();

// Roll number search
void searchByRoll();

//  search by name (part of name)
void searchByName();

// Student update
void updateStudent();

// Soft delete (to status inactive )
void softDeleteStudent();

// Active students list( sorted)
void listActiveStudents();

// load student array from file
int loadStudents(Student students[], int maxSize);

//  to save file from array
void saveStudents(Student students[], int count);

#endif
