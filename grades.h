#ifndef GRADES_H
#define GRADES_H

#include <iostream>
#include <vector>
#include <string>
#include "filehandler.h"
using namespace std;

struct Stats {
    double highest;
    double lowest;
    double mean;
    double median;
};

void enterMarks();
double bestThreeOfFive(double quizzes[], int count);
double computeWeightedTotal(double quizAvg, double assignment, double mid, double final_);
string getLetterGrade(double total);
double computeGPA(const string& roll, int semester);
Stats computeClassState(const string& courseCode);
string applyAttendancePenalty(const string& roll, const string& courseCode, string grade);
void enterMarksMenu();

#endif