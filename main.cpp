#include <iostream>
#include <string>
using namespace std;

#include "filehandler.h"
#include "students_ops.h"
#include "course_ops.h"
#include "attendance.h"
#include "grades.h"
#include "Fee_tracker.h"
#include "Reports.h"

void studentMenu();
void courseMenu();
void attendanceMenu();
void gradesMenu();
void feeMenu();

int choice;
void studentMenu() {
    while (true) {
        cout << "\n========== STUDENT MANAGEMENT ==========" << endl;
        cout << "1. Add New Student" << endl;
        cout << "2. Search by Roll Number" << endl;
        cout << "3. Search by Name" << endl;
        cout << "4. Update Student" << endl;
        cout << "5. Soft Delete Student" << endl;
        cout << "6. List Active Students (Sorted)" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Enter your choice: ";
        cin>>choice;
        switch (choice) {
            case 1: addStudent(); break;
            case 2: searchByRoll(); break;
            case 3: searchByName(); break;
            case 4: updateStudent(); break;
            case 5: softDeleteStudent(); break;
            case 6: listActiveStudents(); break;
            case 0: return;
            default: 
                if (choice != -1) cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void courseMenu() {
    while (true) {
        cout << "\n========== COURSE MANAGEMENT ==========" << endl;
        cout << "1. Enroll Student in Course" << endl;
        cout << "2. Drop Course" << endl;
        cout << "3. List Enrolled Students" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Enter your choice: ";
         cin>>choice;
        switch (choice) {
            case 1: enrollStudentMenu(); break;
            case 2: dropCourseMenu(); break;
            case 3: listEnrolledMenu(); break;
            case 0: return;
            default: 
                if (choice != -1) cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void attendanceMenu() {
    while (true) {
        cout << "\n========== ATTENDANCE MANAGEMENT ==========" << endl;
        cout << "1. Mark Attendance" << endl;
        cout << "2. View Attendance Shortage List" << endl;
        cout << "3. Undo Last Session" << endl;
        cout << "4. Print Daily Sheet" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Enter your choice: ";
         cin>>choice;
        switch (choice) {
            case 1: markAttendance(); break;
            case 2: getShortageList(); break;
            case 3: undoLastSession(); break;
            case 4: printDailySheet(); break;
            case 0: return;
            default: 
                if (choice != -1) cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void gradesMenu() {
    while (true) {
        cout << "\n========== GRADES MANAGEMENT ==========" << endl;
        cout << "1. Enter Marks" << endl;
        cout << "2. Compute Class Statistics" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Enter your choice: ";
        cin>>choice;

        switch (choice) {
            case 1: enterMarksMenu(); break;
            case 2: {
                cout << "Enter Course Code: ";
                string courseCode;
                cin >> courseCode;
                Stats s = computeClassState(courseCode);
                cout << "\n--- Class Statistics for " << courseCode << " ---" << endl;
                cout << "Highest: " << s.highest << endl;
                cout << "Lowest:  " << s.lowest << endl;
                cout << "Mean:    " << s.mean << endl;
                cout << "Median:  " << s.median << endl;
                break;
            }
            case 0: return;
            default: 
                if (choice != -1) cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void feeMenu() {
    while (true) {
        cout << "\n========== FEE MANAGEMENT ==========" << endl;
        cout << "1. Record Payment" << endl;
        cout << "2. Generate Receipt" << endl;
        cout << "3. View Fee Defaulters" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Enter your choice: ";
        cin>>choice;

        switch (choice) {
            case 1: recordPaymentMenu(); break;
            case 2: generateReceiptMenu(); break;
            case 3: getDefaultersMenu(); break;
            case 0: return;
            default: 
                if (choice != -1) cout << "Invalid choice! Please try again." << endl;
        }
    }
}

int main() {
    cout << "============================================================" << endl;
    cout << "         CAMPUS ANALYTICS ENGINE" << endl;
    cout << "         BS AI - Programming Fundamentals Project" << endl;
    cout << "============================================================" << endl;

    while (true) {
        cout << "\n========== MAIN MENU ==========" << endl;
        cout << "1. Student Management" << endl;
        cout << "2. Course Management" << endl;
        cout << "3. Attendance Management" << endl;
        cout << "4. Grades Management" << endl;
        cout << "5. Fee Management" << endl;
        cout << "6. Reports" << endl;
        cout << "0. Exit" << endl;
        cout << "--------------------------------" << endl;
        cout << "Enter your choice: ";
        cin>>choice;

        switch (choice) {
            case 1: studentMenu(); break;
            case 2: courseMenu(); break;
            case 3: attendanceMenu(); break;
            case 4: gradesMenu(); break;
            case 5: feeMenu(); break;
            case 6: reportsMenu(); break;
            case 0:
                cout << "\nThank you for using Campus Analytics Engine!" << endl;
                cout << "Goodbye!" << endl;
                return 0;
            default: 
                if (choice != -1) cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}