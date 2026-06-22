#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include<iostream>
#include<string>
#include "filehandler.h"
#include "student_ops.h"
using namespace std;


// Enrollment result types
enum EnrollResult {
    ENROLL_SUCCESS,
    ENROLL_STUDENT_INACTIVE,
    ENROLL_COURSE_NOT_FOUND,
    ENROLL_NO_SEATS,
    ENROLL_ALREADY_ENROLLED,
    ENROLL_CREDIT_OVERLOAD,
    ENROLL_PREREQ_NOT_MET
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
