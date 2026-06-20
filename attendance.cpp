#include "attendance.h"

// Global backup variables
CSVRow attendanceBackup[500];
int backupRowCount = 0;
int backupColCount = 0;

// ============================================================
// markAttendance: Mark P/A/L for all enrolled students
// ============================================================
void markAttendance() {
    cout << "\n========== MARK ATTENDANCE ==========" << endl;

    string courseCode, date;
    cout << "Enter Course Code: "; cin >> courseCode;
    cout << "Enter Date (DD-MM-YYYY): "; cin >> date;

    if (date.length() != 10 || date[2] != '-' || date[5] != '-') {
        cout << "Error: Date must be DD-MM-YYYY!" << endl;
        return;
    }

    // Get active enrollments for this course
    CSVRow enrollments[200];
    int eCount = 0, eCols = 0;
    readTXT("enrollments.txt", enrollments, eCount, eCols);

    string rolls[50];
    int rollCount = 0;

    for (int i = 0; i < eCount; i++) {
        if (enrollments[i].data[2] == courseCode && enrollments[i].data[5] == "active") {
            rolls[rollCount] = enrollments[i].data[1];
            rollCount++;
        }
    }

    if (rollCount == 0) {
        cout << "No active enrollments!" << endl;
        return;
    }

    // Save backup before changes
    readTXT("attendance_log.txt", attendanceBackup, backupRowCount, backupColCount);

    // Get current max log ID
    CSVRow currentAtt[500];
    int aCount = 0, aCols = 0;
    readTXT("attendance_log.txt", currentAtt, aCount, aCols);

    int maxLogNum = 0;
    for (int i = 0; i < aCount; i++) {
        string id = currentAtt[i].data[0];
        int num = 0;
        for (int j = 1; j < id.length(); j++) {
            if (id[j] >= '0' && id[j] <= '9') {
                num = num * 10 + (id[j] - '0');
            }
        }
        if (num > maxLogNum) maxLogNum = num;
    }

    cout << "\nMark P=Present, A=Absent, L=Late:" << endl;

    for (int i = 0; i < rollCount; i++) {
        CSVRow student;
        string name = "Unknown";
        if (findRow("students.txt", rolls[i], 0, student)) {
            name = student.data[1];
        }

        cout << rolls[i] << " - " << name << " [P/A/L]: ";
        char status;
        cin >> status;

        if (status != 'P' && status != 'p' && status != 'A' && status != 'a' && 
            status != 'L' && status != 'l') {
            cout << "Invalid! Setting to A." << endl;
            status = 'A';
        }

        if (status >= 'a' && status <= 'z') {
            status = status - ('a' - 'A');
        }

        maxLogNum++;

        string logId = "L";
        if (maxLogNum < 10000) logId += "0";
        if (maxLogNum < 1000) logId += "0";
        if (maxLogNum < 100) logId += "0";
        if (maxLogNum < 10) logId += "0";

        string numStr = "";
        int n = maxLogNum;
        while (n > 0) {
            numStr = char('0' + (n % 10)) + numStr;
            n /= 10;
        }
        if (numStr == "") numStr = "0";
        logId += numStr;

        CSVRow row;
        row.colCount = 5;
        row.data[0] = logId;
        row.data[1] = rolls[i];
        row.data[2] = courseCode;
        row.data[3] = date;
        row.data[4] = string(1, status);

        appendTXT("attendance_log.txt", row, 5);
    }

    cout << "Attendance marked!" << endl;
}

// ============================================================
// getAttendancePct: (present + 0.5*late) / total * 100
// ============================================================
double getAttendancePct(string roll, string courseCode) {
    CSVRow attendance[500];
    int aCount = 0, aCols = 0;
    readTXT("attendance_log.txt", attendance, aCount, aCols);

    int present = 0, late = 0, total = 0;

    for (int i = 0; i < aCount; i++) {
        if (attendance[i].data[1] == roll && attendance[i].data[2] == courseCode) {
            total++;
            if (attendance[i].data[4] == "P") present++;
            else if (attendance[i].data[4] == "L") late++;
        }
    }

    if (total == 0) 
    return 0;

    return ((double)present + 0.5 * (double)late) / (double)total * 100;
}

// ============================================================
// getShortageList: Students with < 75%
// ============================================================
void getShortageList() {
    cout << "\n========== ATTENDANCE SHORTAGE ==========" << endl;

    CSVRow enrollments[200];
    int eCount = 0, eCols = 0;
    readTXT("enrollments.txt", enrollments, eCount, eCols);

    bool found = false;
    cout << "Roll No\t\tCourse\t\t%" << endl;
    cout << "--------------------------------" << endl;

    for (int i = 0; i < eCount; i++) {
        if (enrollments[i].data[5] != "active") continue;

        string roll = enrollments[i].data[1];
        string courseCode = enrollments[i].data[2];

        double pct = getAttendancePct(roll, courseCode);

        if (pct < 75.0) {
            found = true;
            cout << roll << "\t\t" << courseCode << "\t\t" << pct << "%" << endl;
        }
    }

    if (!found) 
    cout << "No shortages!" << endl;
}

// ============================================================
// undoLastSession: Restore from backup
// ============================================================
bool undoLastSession() {
    if (backupRowCount == 0) {
        cout << "No backup! Mark attendance first." << endl;
        return false;
    }

    writeTXT("attendance_log.txt", attendanceBackup, backupRowCount, backupColCount);
    backupRowCount = 0;

    cout << "Undo successful!" << endl;
    return true;
}

// ============================================================
// printDailySheet: Show attendance for course on date
// ============================================================
void printDailySheet() {
    cout << "\n========== DAILY SHEET ==========" << endl;

    string courseCode, date;
    cout << "Enter Course: "; cin >> courseCode;
    cout << "Enter Date: "; cin >> date;

    CSVRow attendance[500];
    int aCount = 0, aCols = 0;
    readTXT("attendance_log.txt", attendance, aCount, aCols);

    cout << "\n--- " << courseCode << " on " << date << " ---" << endl;
    cout << "Roll No\t\tStatus" << endl;
    cout << "------------------------" << endl;

    bool found = false;
    for (int i = 0; i < aCount; i++) {
        if (attendance[i].data[2] == courseCode && attendance[i].data[3] == date) {
            found = true;
            cout << attendance[i].data[1] << "\t\t" << attendance[i].data[4] << endl;
        }
    }

    if (!found) cout << "No records found." << endl;
}