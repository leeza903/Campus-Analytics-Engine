#include "grades.h"
#include "attendance.h"

// ============================================================
// bestThreeOfFive: Manual min-finding (no sort)
// ============================================================
double bestThreeOfFive(double quizzes[], int count) {
    if (count <= 0) return 0.0;
    if (count <= 3) {
        double sum = 0.0;
        for (int i = 0; i < count; i++) sum += quizzes[i];
        return sum / count;
    }

    double copy[5];
    for (int i = 0; i < count; i++) copy[i] = quizzes[i];

    int excludeCount = count - 3;

    for (int e = 0; e < excludeCount; e++) {
        int minIdx = 0;
        for (int i = 1; i < count; i++) {
            if (copy[i] < copy[minIdx]) minIdx = i;
        }
        copy[minIdx] = 999.0;
    }

    double sum = 0.0;
    int validCount = 0;
    for (int i = 0; i < count; i++) {
        if (copy[i] < 900.0) {
            sum += copy[i];
            validCount++;
        }
    }

    if (validCount == 0) return 0.0;
    return sum / validCount;
}

// ============================================================
// computeWeightedTotal: Apply weights
// ============================================================
double computeWeightedTotal(double quizAvg, double assignment, double mid, double final_) {
    return quizAvg * 0.10 + assignment * 0.10 + mid * 0.30 + final_ * 0.50;
}

// ============================================================
// getLetterGrade: Map to grade
// ============================================================
string getLetterGrade(double total) {
    if (total >= 85.0) return "A";
    if (total >= 80.0) return "B+";
    if (total >= 70.0) return "B";
    if (total >= 65.0) return "C+";
    if (total >= 60.0) return "C";
    if (total >= 50.0) return "D";
    return "F";
}

// ============================================================
// applyAttendancePenalty: F if < 75%
// ============================================================
string applyAttendancePenalty(const string& roll, const string& courseCode, string grade) {
    double pct = getAttendancePct(roll, courseCode);
    if (pct < 75.0) return "F";
    return grade;
}

// ============================================================
// enterMarks: Input and store marks
// ============================================================
void enterMarks() {
    cout << "\n========== ENTER MARKS ==========" << endl;

    string roll, courseCode;
    cout << "Enter Roll: "; cin >> roll;
    cout << "Enter Course: "; cin >> courseCode;

    // Check student
    CSVRow student;
    if (!findRow("students.txt", roll, 0, student)) {
        cout << "Student not found!" << endl;
        return;
    }
    if (student.data[5] != "active") {
        cout << "Student inactive!" << endl;
        return;
    }

    // Check enrollment
    CSVRow enrollments[200];
    int eCount = 0, eCols = 0;
    readTXT("enrollments.txt", enrollments, eCount, eCols);
    bool isEnrolled = false;
    for (int i = 0; i < eCount; i++) {
        if (enrollments[i].data[1] == roll && enrollments[i].data[2] == courseCode 
            && enrollments[i].data[5] == "active") {
            isEnrolled = true;
            break;
        }
    }
    if (!isEnrolled) {
        cout << "Not enrolled!" << endl;
        return;
    }

    // Input quizzes
    double quizzes[5];
    int quizCount;
    cout << "Number of quizzes (max 5): "; cin >> quizCount;
    if (quizCount < 1) quizCount = 1;
    if (quizCount > 5) quizCount = 5;

    for (int i = 0; i < quizCount; i++) {
        cout << "Quiz " << (i+1) << " (0-100): "; cin >> quizzes[i];
        if (quizzes[i] < 0 || quizzes[i] > 100) {
            cout << "Invalid marks!" << endl;
            return;
        }
    }

    double assignment, mid, final_;
    cout << "Assignment (0-100): "; cin >> assignment;
    if (assignment < 0 || assignment > 100) { cout << "Invalid!" << endl; return; }

    cout << "Mid (0-40): "; cin >> mid;
    if (mid < 0 || mid > 40) { cout << "Invalid!" << endl; return; }

    cout << "Final (0-60): "; cin >> final_;
    if (final_ < 0 || final_ > 60) { cout << "Invalid!" << endl; return; }

    // Compute
    double quizAvg = bestThreeOfFive(quizzes, quizCount);
    double total = computeWeightedTotal(quizAvg, assignment, mid, final_);
    string grade = getLetterGrade(total);
    grade = applyAttendancePenalty(roll, courseCode, grade);

    // Store in grades.txt
    ifstream checkFile("grades.txt");
    bool fileExists = checkFile.is_open();
    checkFile.close();

    if (!fileExists) {
        CSVRow header;
        header.colCount = 8;
        header.data[0] = "roll_no";
        header.data[1] = "course_code";
        header.data[2] = "quiz_avg";
        header.data[3] = "assignment";
        header.data[4] = "mid";
        header.data[5] = "final";
        header.data[6] = "total";
        header.data[7] = "grade";
        appendTXT("grades.txt", header, 8);
    }

    // Helper to convert double to string
    auto numToStr = [](double val) -> string {
        int whole = (int)val;
        int dec = (int)((val - whole) * 100 + 0.5);
        string result = "";
        if (whole == 0) result = "0";
        else {
            string w = "";
            int t = whole;
            while (t > 0) { w = char('0' + (t % 10)) + w; t /= 10; }
            result = w;
        }
        result += ".";
        if (dec < 10) result += "0";
        string dd = "";
        int tt = dec;
        while (tt > 0) { dd = char('0' + (tt % 10)) + dd; tt /= 10; }
        if (dd == "") dd = "0";
        result += dd;
        return result;
    };

    CSVRow row;
    row.colCount = 8;
    row.data[0] = roll;
    row.data[1] = courseCode;
    row.data[2] = numToStr(quizAvg);
    row.data[3] = numToStr(assignment);
    row.data[4] = numToStr(mid);
    row.data[5] = numToStr(final_);
    row.data[6] = numToStr(total);
    row.data[7] = grade;

    appendTXT("grades.txt", row, 8);

    cout << "\n--- Results ---" << endl;
    cout << "Quiz Avg: " << quizAvg << endl;
    cout << "Total:    " << total << endl;
    cout << "Grade:    " << grade << endl;
}

// ============================================================
// computeGPA: Credit-weighted average
// ============================================================
double computeGPA(string roll, int semester) {
    CSVRow grades[200];
    int gCount = 0, gCols = 0;
    readTXT("grades.txt", grades, gCount, gCols);

    CSVRow courses[50];
    int cCount = 0, cCols = 0;
    readTXT("courses.txt", courses, cCount, cCols);

    double totalPoints = 0.0;
    int totalCredits = 0;

    for (int i = 0; i < gCount; i++) {
        if (grades[i].data[0] != roll) continue;

        string courseCode = grades[i].data[1];
        string grade = grades[i].data[7];

        // Find credits
        int credits = 0;
        for (int j = 0; j < cCount; j++) {
            if (courses[j].data[0] == courseCode) {
                for (int k = 0; k < courses[j].data[2].length(); k++) {
                    if (courses[j].data[2][k] >= '0' && courses[j].data[2][k] <= '9') {
                        credits = credits * 10 + (courses[j].data[2][k] - '0');
                    }
                }
                break;
            }
        }

        double points = 0.0;
        if (grade == "A") points = 4.0;
        else if (grade == "B+") points = 3.5;
        else if (grade == "B") points = 3.0;
        else if (grade == "C+") points = 2.5;
        else if (grade == "C") points = 2.0;
        else if (grade == "D") points = 1.0;

        totalPoints += points * credits;
        totalCredits += credits;
    }

    if (totalCredits == 0) return 0.0;
    return totalPoints / totalCredits;
}

// ============================================================
// computeClassState: Stats with bubble sort
// ============================================================
Stats computeClassState(string courseCode) {
    Stats s;
    s.highest = 0.0;
    s.lowest = 100.0;
    s.mean = 0.0;
    s.median = 0.0;

    CSVRow grades[200];
    int gCount = 0, gCols = 0;
    readTXT("grades.txt", grades, gCount, gCols);

    double totals[100];
    int count = 0;

    for (int i = 0; i < gCount && count < 100; i++) {
        if (grades[i].data[1] == courseCode) {
            // Parse total from string
            double total = 0.0;
            bool afterDecimal = false;
            double decimalPlace = 0.1;
            for (int j = 0; j < grades[i].data[6].length(); j++) {
                if (grades[i].data[6][j] == '.') {
                    afterDecimal = true;
                } else if (grades[i].data[6][j] >= '0' && grades[i].data[6][j] <= '9') {
                    if (!afterDecimal) {
                        total = total * 10 + (grades[i].data[6][j] - '0');
                    } else {
                        total += (grades[i].data[6][j] - '0') * decimalPlace;
                        decimalPlace *= 0.1;
                    }
                }
            }

            totals[count] = total;
            count++;

            if (total > s.highest) s.highest = total;
            if (total < s.lowest) s.lowest = total;
        }
    }

    if (count == 0) {
        s.highest = s.lowest = s.mean = s.median = 0.0;
        return s;
    }

    double sum = 0.0;
    for (int i = 0; i < count; i++) sum += totals[i];
    s.mean = sum / count;

    // Bubble sort for median
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (totals[j] > totals[j + 1]) {
                double temp = totals[j];
                totals[j] = totals[j + 1];
                totals[j + 1] = temp;
            }
        }
    }

    if (count % 2 == 0) {
        s.median = (totals[count / 2 - 1] + totals[count / 2]) / 2.0;
    } else {
        s.median = totals[count / 2];
    }

    return s;
}

void enterMarksMenu() {
    enterMarks();
}