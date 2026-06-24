#include "attendance.h"

// ============================================================
// getAttendancePct: Calculate attendance percentage
// ============================================================
double getAttendancePct(string roll, string courseCode) {
    CSVRow attendance[500];
    int aCount = 0, aCols = 0;
    readTXT("attendance_log.txt", attendance, aCount, aCols);

    int total = 0, present = 0;
    for (int i = 0; i < aCount; i++) {
        if (attendance[i].data[1] == roll && attendance[i].data[2] == courseCode) {
            total++;
            if (attendance[i].data[4] == "P") present++;
        }
    }

    if (total == 0) return 0.0;
    return (double)present / total * 100.0;
}

// ============================================================
// markAttendance: Mark attendance for a session
// ============================================================
void markAttendance() {
    cout << "\n========== MARK ATTENDANCE ==========" << endl;

    cin.ignore(10000, '\n');  // Buffer clear

    string courseCode, date;
    cout << "Enter Course Code: ";
    getline(cin, courseCode);
    cout << "Enter Date (DD-MM-YYYY): ";
    getline(cin, date);

    // Check if course exists
    CSVRow course;
    if (!findRow("courses.txt", courseCode, 0, course)) {
        cout << "Error: Course not found!" << endl;
        return;
    }

    // Get enrolled students
    CSVRow enrollments[200];
    int eCount = 0, eCols = 0;
    readTXT("enrollments.txt", enrollments, eCount, eCols);

    // Find last attendance ID
    CSVRow attendance[500];
    int aCount = 0, aCols = 0;
    readTXT("attendance_log.txt", attendance, aCount, aCols);

    int lastId = 0;
    for (int i = 1; i < aCount; i++) {
        string idStr = attendance[i].data[0];
        // Remove 'L' prefix and parse number
        int num = 0;
        for (int j = 1; j < idStr.length(); j++) {
            if (idStr[j] >= '0' && idStr[j] <= '9') {
                num = num * 10 + (idStr[j] - '0');
            }
        }
        if (num > lastId) lastId = num;
    }

    cout << "\nMark attendance (P for Present, A for Absent):" << endl;
    cout << "Roll No\t\tName\t\tStatus" << endl;
    cout << "----------------------------------------" << endl;

    bool anyMarked = false;

    for (int i = 0; i < eCount; i++) {
        if (enrollments[i].data[2] != courseCode) continue;
        if (enrollments[i].data[5] != "active") continue;

        string roll = enrollments[i].data[1];

        // Get student name
        CSVRow student;
        string name = "Unknown";
        if (findRow("students.txt", roll, 0, student)) {
            name = student.data[1];
        }

        cout << roll << "\t" << name << "\t";

        char status;
        cin >> status;
        cin.ignore(10000, '\n');  // Clear buffer

        if (status != 'P' && status != 'p' && status != 'A' && status != 'a') {
            cout << "Invalid input! Skipping..." << endl;
            continue;
        }

        string statusStr = (status == 'P' || status == 'p') ? "P" : "A";

        // Create new attendance record
        lastId++;
        string newId = "L";
        if (lastId < 10000) newId += "0";
        if (lastId < 1000) newId += "0";
        if (lastId < 100) newId += "0";
        if (lastId < 10) newId += "0";

        string numStr = "";
        int n = lastId;
        while (n > 0) {
            numStr = char('0' + (n % 10)) + numStr;
            n /= 10;
        }
        if (numStr == "") numStr = "0";
        newId += numStr;

        CSVRow row;
        row.colCount = 5;
        row.data[0] = newId;
        row.data[1] = roll;
        row.data[2] = courseCode;
        row.data[3] = date;
        row.data[4] = statusStr;

        appendTXT("attendance_log.txt", row, 5);
        anyMarked = true;
    }

    if (anyMarked) {
        cout << "\nAttendance marked successfully!" << endl;
    } else {
        cout << "\nNo active students found for this course." << endl;
    }
}

// ============================================================
// getShortageList: Students with < 75% attendance
// ============================================================
void getShortageList() {
    cout << "\n========== ATTENDANCE SHORTAGE LIST ==========" << endl;

    cin.ignore(10000, '\n');  // Buffer clear

    string courseCode;
    cout << "Enter Course Code: ";
    getline(cin, courseCode);

    CSVRow enrollments[200];
    int eCount = 0, eCols = 0;
    readTXT("enrollments.txt", enrollments, eCount, eCols);

    bool found = false;
    cout << "Roll No\t\tName\t\tAttendance %" << endl;
    cout << "------------------------------------------------" << endl;

    for (int i = 0; i < eCount; i++) {
        if (enrollments[i].data[2] != courseCode) continue;
        if (enrollments[i].data[5] != "active") continue;

        string roll = enrollments[i].data[1];
        double pct = getAttendancePct(roll, courseCode);

        if (pct < 75.0) {
            found = true;
            CSVRow student;
            string name = "Unknown";
            if (findRow("students.txt", roll, 0, student)) {
                name = student.data[1];
            }
            cout << roll << "\t\t" << name << "\t\t" << pct << "%" << endl;
        }
    }

    if (!found) {
        cout << "No attendance shortage found!" << endl;
    }
}

// ============================================================
// undoLastSession: Remove last attendance session
// ============================================================
void undoLastSession() {
    cout << "\n========== UNDO LAST SESSION ==========" << endl;

    cin.ignore(10000, '\n');  // Buffer clear

    string courseCode, date;
    cout << "Enter Course Code: ";
    getline(cin, courseCode);
    cout << "Enter Date: ";
    getline(cin, date);

    CSVRow attendance[500];
    int aCount = 0, aCols = 0;
    readTXT("attendance_log.txt", attendance, aCount, aCols);

    // Find records to delete (last session for this course and date)
    bool deleted = false;
    CSVRow newAttendance[500];
    int newCount = 0;

    for (int i = 0; i < aCount; i++) {
        if (attendance[i].data[2] == courseCode && attendance[i].data[3] == date) {
            deleted = true;
            continue;  // Skip this record
        }
        newAttendance[newCount] = attendance[i];
        newCount++;
    }

    if (deleted) {
        writeTXT("attendance_log.txt", newAttendance, newCount, aCols);
        cout << "Last session attendance deleted!" << endl;
    } else {
        cout << "No attendance found for this course and date." << endl;
    }
}

// ============================================================
// printDailySheet: Show attendance for course on date
// Direct file read - no MAX_ROWS limit
// ============================================================
void printDailySheet() {
    cout << "\n========== DAILY SHEET ==========" << endl;

    cin.ignore(10000, '\n');  // Buffer clear

    string courseCode, date;
    cout << "Enter Course: ";
    getline(cin, courseCode);
    cout << "Enter Date: ";
    getline(cin, date);

    // Direct file read - no MAX_ROWS limit!
    ifstream file("attendance_log.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open file!" << endl;
        return;
    }

    string header;
    getline(file, header);  // Skip header

    cout << "\n--- " << courseCode << " on " << date << " ---" << endl;
    cout << "Roll No\t\t Status" << endl;
    cout << "------------------------" << endl;

    bool found = false;
    string line;
    while (getline(file, line)) {
        // Parse CSV line manually
        string fields[5];
        int fieldIdx = 0;
        string current = "";

        for (int i = 0; i < line.length() && fieldIdx < 5; i++) {
            if (line[i] == ',') {
                fields[fieldIdx] = current;
                current = "";
                fieldIdx++;
            } else {
                current += line[i];
            }
        }
        fields[fieldIdx] = current;

        // Check if course and date match
        if (fields[2] == courseCode && fields[3] == date) {
            found = true;
            cout << fields[1] << "\t\t" << fields[4] << endl;
        }
    }

    file.close();
    if (!found) cout << "No records found." << endl;
}