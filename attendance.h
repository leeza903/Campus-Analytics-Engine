#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include "filehandler.h"

// Global backup for undo
extern CSVRow attendanceBackup[500];
extern int backupRowCount;
extern int backupColCount;

// Mark attendance for a course on a date
void markAttendance();

// Get attendance percentage
double getAttendancePct(string roll, string courseCode);

// Get students with < 75% attendance
void getShortageList();

// Undo last session
bool undoLastSession();

// Print daily sheet
void printDailySheet();

#endif

