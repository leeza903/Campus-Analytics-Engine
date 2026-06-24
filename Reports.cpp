#include "reports.h"

void printMeritList() {
    const int MAX = 100;
    Student students[MAX];
    int count = loadStudents(students, MAX);

    Student active[MAX];
    int activeCount = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].status == "active") {
            active[activeCount] = students[i];
            activeCount++;
        }
    }

    if (activeCount == 0) {
        cout << "No active students found." << endl;
        return;
    }

    for (int i = 0; i < activeCount - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < activeCount; j++) {
            if (active[j].cgpa > active[maxIdx].cgpa) {
                maxIdx = j;
            }
        }
        Student temp = active[i];
        active[i] = active[maxIdx];
        active[maxIdx] = temp;
    }

    cout << "\n========== MERIT LIST ==========" << endl;
    cout << "Rank\tRoll No\t\tName\t\t\tCGPA" << endl;
    cout << "----------------------------------------------------" << endl;
    for (int i = 0; i < activeCount; i++) {
        cout << (i + 1) << "\t" << active[i].roll_no << "\t" 
             << active[i].name << "\t\t" << active[i].cgpa << endl;
    }
}

void printAttendanceDefaulters() {
    cout << "\n========== ATTENDANCE DEFAULTERS ==========" << endl;
    cout << "Roll No\t\tName\t\t\tCourse\t\tAttendance %" << endl;
    cout << "----------------------------------------------------------------" << endl;

    CSVRow enrollments[MAX_ROWS];
    int enrollCount = 0, enrollColCount = 0;
    readTXT("enrollments.txt", enrollments, enrollCount, enrollColCount);
    bool found = false;

    for (int i = 1; i < enrollCount; i++) {
        if (enrollments[i].colCount < 6) continue;
        if (enrollments[i].data[5] != "active") continue;

        string roll = enrollments[i].data[1];
        string courseCode = enrollments[i].data[2];

        double pct = getAttendancePct(roll, courseCode);

        if (pct < 75.0) {
            found = true;
            CSVRow student;
            bool foundStudent = findRow("students.txt", roll, 0, student);
            string name = (student.colCount > 1) ? student.data[1] : "Unknown";

            cout << roll << "\t\t" << name << "\t\t" << courseCode << "\t\t" << pct << "%" << endl;
        }
    }

    if (!found) {
        cout << "No attendance defaulters found!" << endl;
    }
}

void printFeeDefaulters() {
    getDefaulters();
}

void printSemesterResult() {
    cout << "\n========== SEMESTER RESULT SHEET ==========" << endl;
    cout << "Enter Semester: ";
    int semester;
    cin >> semester;

    const int MAX = 100;
    Student students[MAX];
    int count = loadStudents(students, MAX);

    cout << "\n+------------+------------------+-------+-------+----------------+" << endl;
    cout << "| Roll No    | Name             | GPA   | Grade | Attendance     |" << endl;
    cout << "+------------+------------------+-------+-------+----------------+" << endl;

    for (int i = 0; i < count; i++) {
        if (students[i].status != "active") continue;
        if (students[i].semester != semester) continue;

        double gpa = computeGPA(students[i].roll_no, semester);

        string overallGrade = "F";
        if (gpa >= 3.7) overallGrade = "A";
        else if (gpa >= 3.3) overallGrade = "B+";
        else if (gpa >= 3.0) overallGrade = "B";
        else if (gpa >= 2.7) overallGrade = "C+";
        else if (gpa >= 2.0) overallGrade = "C";
        else if (gpa >= 1.0) overallGrade = "D";

        CSVRow enrollments[MAX_ROWS];
        int enrollCount = 0, enrollColCount = 0;
        readTXT("enrollments.txt", enrollments, enrollCount, enrollColCount);
        bool shortage = false;
        for (int j = 1; j < enrollCount; j++) {
            if (enrollments[j].colCount > 2 && enrollments[j].data[1] == students[i].roll_no && 
                enrollments[j].data[5] == "active") {
                double pct = getAttendancePct(students[i].roll_no, enrollments[j].data[2]);
                if (pct < 75.0) {
                    shortage = true;
                    break;
                }
            }
        }

        string attStatus = shortage ? "SHORTAGE" : "OK";

        cout << "| " << left << setw(10) << students[i].roll_no 
             << " | " << setw(16) << students[i].name.substr(0, 16)
             << " | " << setw(5) << fixed << setprecision(2) << gpa
             << " | " << setw(5) << overallGrade
             << " | " << setw(14) << attStatus << " |" << endl;
    }

    cout << "+------------+------------------+-------+-------+----------------+" << endl;
}

void printDepartmentSummary() {
    const int MAX = 100;
    Student students[MAX];
    int count = loadStudents(students, MAX);

    string departments[20];
    int deptCount = 0;

    for (int i = 0; i < count; i++) {
        bool exists = false;
        for (int j = 0; j < deptCount; j++) {
            if (students[i].department == departments[j]) {
                exists = true;
                break;
            }
        }
        if (!exists && deptCount < 20) {
            departments[deptCount] = students[i].department;
            deptCount++;
        }
    }

    cout << "\n========== DEPARTMENT SUMMARY ==========" << endl;
    cout << "Department\t\tCount\tAvg CGPA\tPass Rate" << endl;
    cout << "--------------------------------------------------------" << endl;

    for (int d = 0; d < deptCount; d++) {
        int deptStudents = 0;
        double totalCGPA = 0.0;
        int passed = 0;

        for (int i = 0; i < count; i++) {
            if (students[i].department == departments[d]) {
                deptStudents++;
                totalCGPA += students[i].cgpa;
                if (students[i].cgpa >= 2.0) {
                    passed++;
                }
            }
        }

        double avgCGPA = (deptStudents > 0) ? totalCGPA / deptStudents : 0.0;
        double passRate = (deptStudents > 0) ? ((double)passed / deptStudents * 100.0) : 0.0;

        cout << departments[d] << "\t\t" << deptStudents << "\t" 
             << fixed << setprecision(2) << avgCGPA << "\t\t" 
             << fixed << setprecision(1) << passRate << "%" << endl;
    }
}

void exportReportToFile() {
    cout << "\n========== EXPORT REPORT ==========" << endl;
    cout << "Select report to export:" << endl;
    cout << "1. Merit List" << endl;
    cout << "2. Attendance Defaulters" << endl;
    cout << "3. Fee Defaulters" << endl;
    cout << "4. Semester Result" << endl;
    cout << "5. Department Summary" << endl;
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    cout << "Enter filename to save (e.g., report.txt): ";
    string filename;
    cin >> filename;

    streambuf* originalBuffer = cout.rdbuf();

    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Error: Could not create file!" << endl;
        return;
    }

    cout.rdbuf(outFile.rdbuf());

    switch (choice) {
        case 1:
            printMeritList();
            break;
        case 2:
            printAttendanceDefaulters();
            break;
        case 3:
            printFeeDefaulters();
            break;
        case 4:
            printSemesterResult();
            break;
        case 5:
            printDepartmentSummary();
            break;
        default:
            cout << "Invalid choice!" << endl;
            break;
    }

    cout.rdbuf(originalBuffer);
    outFile.close();

    cout << "Report exported to " << filename << " successfully!" << endl;
}

void reportsMenu() {
    while (true) {
        cout << "\n========== REPORTS MENU ==========" << endl;
        cout << "1. Print Merit List" << endl;
        cout << "2. Print Attendance Defaulters" << endl;
        cout << "3. Print Fee Defaulters" << endl;
        cout << "4. Print Semester Result" << endl;
        cout << "5. Print Department Summary" << endl;
        cout << "6. Export Report to File" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: printMeritList(); break;
            case 2: printAttendanceDefaulters(); break;
            case 3: printFeeDefaulters(); break;
            case 4: printSemesterResult(); break;
            case 5: printDepartmentSummary(); break;
            case 6: exportReportToFile(); break;
            case 0: return;
            default: cout << "Invalid choice!" << endl;
        }
    }
}