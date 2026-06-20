#include "course_ops.h"

// ============================================================
// getCourse: Get course details by code
// ============================================================
CSVRow getCourseRow(string courseCode) {
    CSVRow rows[50];
    int rowCount = 0, colCount = 0;
    readTXT("courses.txt", rows, rowCount, colCount);

    for (int i = 0; i < rowCount; i++) {
        if (rows[i].data[0] == courseCode) {
            return rows[i];
        }
    }

    CSVRow empty;
    empty.colCount = 0;
    return empty;
}

// ============================================================
// getCreditLoad: Sum credit hours of active enrollments
// ============================================================
int getCreditLoad(string roll, int semester) {
    CSVRow enrollments[200];
    int eCount = 0, eCols = 0;
    readTXT("enrollments.txt", enrollments, eCount, eCols);

    CSVRow courses[50];
    int cCount = 0, cCols = 0;
    readTXT("courses.txt", courses, cCount, cCols);

    int totalCredits = 0;

    for (int i = 0; i < eCount; i++) {
        // Check roll match
        if (enrollments[i].data[1] != roll) continue;

        // Check semester match
        int enrollSem = 0;
        for (int j = 0; j < enrollments[i].data[3].length(); j++) {
            if (enrollments[i].data[3][j] >= '0' && enrollments[i].data[3][j] <= '9') {
                enrollSem = enrollSem * 10 + (enrollments[i].data[3][j] - '0');
            }
        }
        if (enrollSem != semester) continue;

        // Check status active
        if (enrollments[i].data[5] != "active") continue;

        // Find course credits
        string courseCode = enrollments[i].data[2];
        for (int j = 0; j < cCount; j++) {
            if (courses[j].data[0] == courseCode) {
                int credits = 0;
                for (int k = 0; k < courses[j].data[2].length(); k++) {
                    if (courses[j].data[2][k] >= '0' && courses[j].data[2][k] <= '9') {
                        credits = credits * 10 + (courses[j].data[2][k] - '0');
                    }
                }
                totalCredits += credits;
                break;
            }
        }
    }

    return totalCredits;
}

// ============================================================
// checkPrerequisite: Check if student passed prerequisite
// ============================================================
bool checkPrerequisite(string roll, string courseCode) {
    CSVRow course = getCourseRow(courseCode);
    if (course.colCount == 0) return false;

    string prereq = course.data[6];  // prerequisite column
    if (prereq == "NONE") return true;

    // Check if student enrolled in prerequisite
    CSVRow enrollments[200];
    int eCount = 0, eCols = 0;
    readTXT("enrollments.txt", enrollments, eCount, eCols);

    for (int i = 0; i < eCount; i++) {
        if (enrollments[i].data[1] == roll && enrollments[i].data[2] == prereq) {
            return true;
        }
    }

    return false;
}

// ============================================================
// enrollStudent: Main enrollment with all checks
// ============================================================
EnrollResult enrollStudent(string roll, string courseCode, int semester) {
    // 1. Check student active
    CSVRow student;
    if (!findRow("students.txt", roll, 0, student)) {
        return STUDENT_INACTIVE;
    }
    if (student.data[5] != "active") return STUDENT_INACTIVE;

    // 2. Check course exists
    CSVRow course = getCourseRow(courseCode);
    if (course.colCount == 0) return COURSE_NOT_FOUND;

    // 3. Check seats
    int capacity = 0, enrolled = 0;
    for (int i = 0; i < course.data[4].length(); i++) {
        if (course.data[4][i] >= '0' && course.data[4][i] <= '9') {
            capacity = capacity * 10 + (course.data[4][i] - '0');
        }
    }
    for (int i = 0; i < course.data[5].length(); i++) {
        if (course.data[5][i] >= '0' && course.data[5][i] <= '9') {
            enrolled = enrolled * 10 + (course.data[5][i] - '0');
        }
    }
    if (enrolled >= capacity) return NO_SEATS;

    // 4. Check not already enrolled
    CSVRow enrollments[200];
    int eCount = 0, eCols = 0;
    readTXT("enrollments.txt", enrollments, eCount, eCols);
    for (int i = 0; i < eCount; i++) {
        if (enrollments[i].data[1] == roll && enrollments[i].data[2] == courseCode) {
            return ALREADY_ENROLLED;
        }
    }

    // 5. Check credit load <= 21
    int currentLoad = getCreditLoad(roll, semester);
    int courseCredits = 0;
    for (int i = 0; i < course.data[2].length(); i++) {
        if (course.data[2][i] >= '0' && course.data[2][i] <= '9') {
            courseCredits = courseCredits * 10 + (course.data[2][i] - '0');
        }
    }
    if (currentLoad + courseCredits > 21) return CREDIT_OVERLOAD;

    // 6. Check prerequisite
    if (!checkPrerequisite(roll, courseCode)) return PREREQ_NOT_MET;

    // All checks passed - create enrollment
    int maxId = 0;
    for (int i = 0; i < eCount; i++) {
        string idStr = enrollments[i].data[0];
        int num = 0;
        for (int j = 1; j < idStr.length(); j++) {
            if (idStr[j] >= '0' && idStr[j] <= '9') {
                num = num * 10 + (idStr[j] - '0');
            }
        }
        if (num > maxId) maxId = num;
    }
    maxId++;

    // Format new ID
    string newId = "E";
    if (maxId < 1000) newId += "0";
    if (maxId < 100) newId += "0";
    if (maxId < 10) newId += "0";

    string numStr = "";
    int n = maxId;
    while (n > 0) {
        numStr = char('0' + (n % 10)) + numStr;
        n /= 10;
    }
    if (numStr == "") numStr = "0";
    newId += numStr;

    // Semester to string
    string semStr = "";
    int s = semester;
    while (s > 0) {
        semStr = char('0' + (s % 10)) + semStr;
        s /= 10;
    }
    if (semStr == "") semStr = "0";

    CSVRow row;
    row.colCount = 6;
    row.data[0] = newId;
    row.data[1] = roll;
    row.data[2] = courseCode;
    row.data[3] = semStr;
    row.data[4] = "15-01-2024";
    row.data[5] = "active";

    appendTXT("enrollments.txt", row, 6);

    // Update enrolled count in courses.txt
    CSVRow courses[50];
    int cCount = 0, cCols = 0;
    readTXT("courses.txt", courses, cCount, cCols);
    for (int i = 0; i < cCount; i++) {
        if (courses[i].data[0] == courseCode) {
            int newEnrolled = enrolled + 1;
            string enrolledStr = "";
            int e = newEnrolled;
            while (e > 0) {
                enrolledStr = char('0' + (e % 10)) + enrolledStr;
                e /= 10;
            }
            if (enrolledStr == "") enrolledStr = "0";
            courses[i].data[5] = enrolledStr;
            break;
        }
    }
    writeTXT("courses.txt", courses, cCount, cCols);

    return SUCCESS;
}

// ============================================================
// dropCourse: Drop course (if no attendance records)
// ============================================================
bool dropCourse(string roll, string courseCode, int semester) {
    // Check attendance records
    CSVRow attendance[500];
    int aCount = 0, aCols = 0;
    readTXT("attendance_log.txt", attendance, aCount, aCols);

    for (int i = 0; i < aCount; i++) {
        if (attendance[i].data[1] == roll && attendance[i].data[2] == courseCode) {
            cout << "Cannot drop: Attendance records exist!" << endl;
            return false;
        }
    }

    // Update enrollment to dropped
    CSVRow enrollments[200];
    int eCount = 0, eCols = 0;
    readTXT("enrollments.txt", enrollments, eCount, eCols);

    bool found = false;
    for (int i = 0; i < eCount; i++) {
        if (enrollments[i].data[1] == roll && enrollments[i].data[2] == courseCode) {
            enrollments[i].data[5] = "dropped";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Enrollment not found!" << endl;
        return false;
    }

    writeTXT("enrollments.txt", enrollments, eCount, eCols);

    // Decrease enrolled count
    CSVRow courses[50];
    int cCount = 0, cCols = 0;
    readTXT("courses.txt", courses, cCount, cCols);
    for (int i = 0; i < cCount; i++) {
        if (courses[i].data[0] == courseCode) {
            int enrolled = 0;
            for (int j = 0; j < courses[i].data[5].length(); j++) {
                if (courses[i].data[5][j] >= '0' && courses[i].data[5][j] <= '9') {
                    enrolled = enrolled * 10 + (courses[i].data[5][j] - '0');
                }
            }
            if (enrolled > 0) enrolled--;
            string enrolledStr = "";
            int e = enrolled;
            while (e > 0) {
                enrolledStr = char('0' + (e % 10)) + enrolledStr;
                e /= 10;
            }
            if (enrolledStr == "") enrolledStr = "0";
            courses[i].data[5] = enrolledStr;
            break;
        }
    }
    writeTXT("courses.txt", courses, cCount, cCols);

    return true;
}

// ============================================================
// listEnrolledStudents: Show active students in course
// ============================================================
void listEnrolledStudents(string courseCode, int semester) {
    CSVRow enrollments[200];
    int eCount = 0, eCols = 0;
    readTXT("enrollments.txt", enrollments, eCount, eCols);

    cout << "\n--- Enrolled in " << courseCode << " (Sem " << semester << ") ---" << endl;
    cout << "Roll No\t\tName" << endl;
    cout << "------------------------" << endl;

    bool found = false;
    for (int i = 0; i < eCount; i++) {
        if (enrollments[i].data[2] != courseCode) continue;
        if (enrollments[i].data[5] != "active") continue;

        int enrollSem = 0;
        for (int j = 0; j < enrollments[i].data[3].length(); j++) {
            if (enrollments[i].data[3][j] >= '0' && enrollments[i].data[3][j] <= '9') {
                enrollSem = enrollSem * 10 + (enrollments[i].data[3][j] - '0');
            }
        }
        if (enrollSem != semester) continue;

        CSVRow student;
        string name = "Unknown";
        if (findRow("students.txt", enrollments[i].data[1], 0, student)) {
            name = student.data[1];
        }

        cout << enrollments[i].data[1] << "\t\t" << name << endl;
        found = true;
    }

    if (!found) 
    cout << "No active enrollments." << endl;
}

// ============================================================
// Menu wrappers
// ============================================================
void enrollStudentMenu() {
    cout << "\n========== ENROLL STUDENT ==========" << endl;
    string roll, courseCode;
    int semester;

    cout << "Enter Roll: "; cin >> roll;
    cout << "Enter Course: "; cin >> courseCode;
    cout << "Enter Semester: "; cin >> semester;

    EnrollResult result = enrollStudent(roll, courseCode, semester);

    switch (result) {
        case SUCCESS:
         cout << "Enrolled successfully!" << endl; break;
        case STUDENT_INACTIVE:
         cout << "Error: Student inactive!" << endl; break;
        case COURSE_NOT_FOUND:
         cout << "Error: Course not found!" << endl; break;
        case NO_SEATS: 
        cout << "Error: No seats!" << endl; break;
        case ALREADY_ENROLLED:
         cout << "Error: Already enrolled!" << endl; break;
        case CREDIT_OVERLOAD: 
        cout << "Error: Credit > 21!" << endl; break;
        case PREREQ_NOT_MET:
         cout << "Error: Prerequisite not met!" << endl; break;
    }
}

void dropCourseMenu() {
    cout << "\n========== DROP COURSE ==========" << endl;
    string roll, courseCode;
    int semester;

    cout << "Enter Roll: "; cin >> roll;
    cout << "Enter Course: "; cin >> courseCode;
    cout << "Enter Semester: "; cin >> semester;

    if (dropCourse(roll, courseCode, semester)) {
        cout << "Course dropped!" << endl;
    }
}

void listEnrolledMenu() 
{
    cout << "\n========== LIST ENROLLED ==========" << endl;
    string courseCode;
    int semester;

    cout << "Enter Course: "; cin >> courseCode;
    cout << "Enter Semester: "; cin >> semester;

    listEnrolledStudents(courseCode, semester);
}