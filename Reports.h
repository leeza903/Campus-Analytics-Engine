#ifndef REPORTS_H
#define REPORTS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include "filehandler.h"
#include "student_ops.h"
#include "grades.h"
#include "attendance.h"
#include "fee_tracker.h"
using namespace std;

void printMeritList();
void printAttendanceDefaulters();
void printFeeDefaulters();
void printSemesterResult();
void printDepartmentSummary();
void exportReportToFile();
void reportsMenu();

#endif
