# Campus Analytics Engine

A multi-file, menu-driven data analytics system for a fictional university campus.
Built for Programming Fundamentals (Semester 2) using only basic C++ constructs.

## Project Structure
campus_analytics_engine/
├── main.cpp


├── filehandler.h / filehandler.cpp

├── student_ops.h / student_ops.cpp

├── course_ops.h / course_ops.cpp

├── attendance.h / attendance.cpp

├── grades.h / grades.cpp

├── fee_tracker.h / fee_tracker.cpp

├── reports.h / reports.cpp

├── students.txt

├── courses.txt

├── enrollments.txt

├── attendance_log.txt

└── fees.txt

## How to Compile

```bash
g++ *.cpp -o campus
How to Run
./campus

Features:
Student Management (Add, Search, Update, Soft Delete)
Course Enrollment (with prerequisite & credit load checks)
Attendance Tracking (mark, compute %, flag shortages, undo)
Grade Management (weighted totals, letter grades, GPA, class stats)
Fee Tracking (payments, late fines, receipts, defaulters)
Reports (merit list, defaulters, semester results, department summary)

Rules Followed:
No class keyword (only struct)
No STL algorithms (#include <algorithm>)
No map, set, unordered_map
No ctime (manual date arithmetic)
Multi-file structure with header files
Manual string parsing and sorting algorithms