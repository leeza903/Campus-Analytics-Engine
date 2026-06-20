#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include "filehandler.h"

// Enrollment result types
enum EnrollResult {
    SUCCESS,
    STUDENT_INACTIVE,
    COURSE_NOT_FOUND,
    NO_SEATS,
    ALREADY_ENROLLED,
    CREDIT_OVERLOAD,
    PREREQ_NOT_MET
};

// Enroll student in course
EnrollResult enrollStudent(string roll, string courseCode, int semester);

// Drop course
bool dropCourse(string roll, string courseCode, int semester);

// Get total credit hours for student
int getCreditLoad(string roll, int semester);

// Check prerequisite
bool checkPrerequisite(string roll, string courseCode);

// List enrolled students
void listEnrolledStudents(string courseCode, int semester);

// Menu functions
void enrollStudentMenu();
void dropCourseMenu();
void listEnrolledMenu();

#endif
