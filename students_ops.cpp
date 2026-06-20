#include "students_ops.h"

// ============================================================
// isValidRollFormat: BSAI-YY-XXX check
// Example: BSAI-23-001 ✓  |  ABC-23-001 ✗
// ============================================================
bool isValidRollFormat(string roll) {
    // Length exactly 11
    if (roll.length() != 11) return false;

    // "BSAI" must written
    if (roll[0] != 'B' || roll[1] != 'S' || roll[2] != 'A' || roll[3] != 'I')
        return false;

    // 4th position dash
    if (roll[4] != '-') return false;

    // 5th aur 6th position  digits
    if (roll[5] < '0' || roll[5] > '9') return false;
    if (roll[6] < '0' || roll[6] > '9') return false;

    // 7th position  dash
    if (roll[7] != '-') return false;

    // Last 3 digits
    if (roll[8] < '0' || roll[8] > '9') return false;
    if (roll[9] < '0' || roll[9] > '9') return false;
    if (roll[10] < '0' || roll[10] > '9') return false;

    return true;
}

// ============================================================
// hasDigits
// ============================================================
bool hasDigits(string name) {
    for (int i = 0; i < name.length(); i++) {
        if (name[i] >= '0' && name[i] <= '9') {
            return true;
        }
    }
    return false;
}

// ============================================================
// loadStudents: from file to array
// ============================================================
int loadStudents(Student students[], int maxSize) {
    CSVRow rows[MAX_ROWS];
    int rowCount = 0, colCount = 0;

    // File read
    readTXT("students.txt", rows, rowCount, colCount);

    int count = 0;
    for (int i = 0; i < rowCount && count < maxSize; i++) {
        // access Row  data
        students[count].roll_no = rows[i].data[0];
        students[count].name = rows[i].data[1];
        students[count].department = rows[i].data[2];

        // Semester string to int conversion
        students[count].semester = 0;
        for (int j = 0; j < rows[i].data[3].length(); j++) {
            students[count].semester = students[count].semester * 10 + (rows[i].data[3][j] - '0');
        }

        // CGPA string to double conversion
        students[count].cgpa = 0.0;
        bool afterDecimal = false;
        double decimalPlace = 0.1;
        for (int j = 0; j < rows[i].data[4].length(); j++) {
            if (rows[i].data[4][j] == '.') {
                afterDecimal = true;
            } else if (rows[i].data[4][j] >= '0' && rows[i].data[4][j] <= '9') {
                if (!afterDecimal) {
                    students[count].cgpa = students[count].cgpa * 10 + (rows[i].data[4][j] - '0');
                } else {
                    students[count].cgpa += (rows[i].data[4][j] - '0') * decimalPlace;
                    decimalPlace *= 0.1;
                }
            }
        }

        students[count].status = rows[i].data[5];
        count++;
    }

    return count;
}

// ============================================================
// saveStudents: save file from array
// ============================================================
void saveStudents(Student students[], int count) {
    CSVRow rows[MAX_ROWS];

    // Header row
    rows[0].colCount = 6;
    rows[0].data[0] = "roll_no";
    rows[0].data[1] = "name";
    rows[0].data[2] = "department";
    rows[0].data[3] = "semester";
    rows[0].data[4] = "cgpa";
    rows[0].data[5] = "status";

    // students in rows
    for (int i = 0; i < count; i++) {
        rows[i + 1].colCount = 6;
        rows[i + 1].data[0] = students[i].roll_no;
        rows[i + 1].data[1] = students[i].name;
        rows[i + 1].data[2] = students[i].department;

        // Semester int to string
        string semStr = "";
        int sem = students[i].semester;
        if (sem == 0) semStr = "0";
        else {
            while (sem > 0) {
                semStr = char('0' + (sem % 10)) + semStr;
                sem /= 10;
            }
        }
        rows[i + 1].data[3] = semStr;

        // CGPA double to string (2 decimal places)
        string cgpaStr = "";
        int whole = (int)students[i].cgpa;
        int decimal = (int)((students[i].cgpa - whole) * 100 + 0.5);

        // Whole part
        if (whole == 0) cgpaStr = "0";
        else {
            string wStr = "";
            int w = whole;
            while (w > 0) {
                wStr = char('0' + (w % 10)) + wStr;
                w /= 10;
            }
            cgpaStr = wStr;
        }
        cgpaStr += ".";
        if (decimal < 10) cgpaStr += "0";
        string dStr = "";
        int d = decimal;
        while (d > 0) {
            dStr = char('0' + (d % 10)) + dStr;
            d /= 10;
        }
        if (dStr == "") dStr = "0";
        cgpaStr += dStr;

        rows[i + 1].data[4] = cgpaStr;
        rows[i + 1].data[5] = students[i].status;
    }

    //Write in  File  (count + 1 bcz  header  also counted)
    writeTXT("students.txt", rows, count + 1, 6);
}

// ============================================================
// addStudent: new student addition
// ============================================================
void addStudent() {
    Student s;

    cout << "\n========== ADD NEW STUDENT ==========" << endl;

    // Roll number input
    cout << "Enter Roll Number (format: BSAI-YY-XXX): ";
    cin >> s.roll_no;

    // Format check
    if (!isValidRollFormat(s.roll_no)) {
        cout << "Error: Invalid roll format! Must be BSAI-YY-XXX." << endl;
        return;
    }

    // Duplicate check
    if (rowExists("students.txt", s.roll_no, 0)) {
        cout << "Error: Student with this roll number already exists!" << endl;
        return;
    }

    // Name input
    cout << "Enter Name: ";
    cin.ignore();  // Buffer clear karo
    getline(cin, s.name);

    // Name mein digits check
    if (hasDigits(s.name)) {
        cout << "Error: Name cannot contain digits!" << endl;
        return;
    }

    // Department
    cout << "Enter Department: ";
    getline(cin, s.department);

    // Semester
    cout << "Enter Semester: ";
    string semInput;
    getline(cin, semInput);
    s.semester = 0;
    for (int i = 0; i < semInput.length(); i++) {
        if (semInput[i] >= '0' && semInput[i] <= '9') {
            s.semester = s.semester * 10 + (semInput[i] - '0');
        }
    }

    // CGPA
    cout << "Enter CGPA (0.0 to 4.0): ";
    string cgpaInput;
    getline(cin, cgpaInput);

    s.cgpa = 0.0;
    bool afterDecimal = false;
    double decimalPlace = 0.1;
    for (int i = 0; i < cgpaInput.length(); i++) {
        if (cgpaInput[i] == '.') {
            afterDecimal = true;
        } else if (cgpaInput[i] >= '0' && cgpaInput[i] <= '9') {
            if (!afterDecimal) {
                s.cgpa = s.cgpa * 10 + (cgpaInput[i] - '0');
            } else {
                s.cgpa += (cgpaInput[i] - '0') * decimalPlace;
                decimalPlace *= 0.1;
            }
        }
    }

    if (s.cgpa < 0.0 || s.cgpa > 4.0) {
        cout << "Error: CGPA must be between 0.0 and 4.0!" << endl;
        return;
    }

    s.status = "active";

    //  append in  file
    CSVRow row;
    row.colCount = 6;
    row.data[0] = s.roll_no;
    row.data[1] = s.name;
    row.data[2] = s.department;

    // Semester to string
    string semStr = "";
    int sem = s.semester;
    if (sem == 0) semStr = "0";
    else {
        while (sem > 0) {
            semStr = char('0' + (sem % 10)) + semStr;
            sem /= 10;
        }
    }
    row.data[3] = semStr;

    // CGPA to string
    string cgpaStr = "";
    int whole = (int)s.cgpa;
    int decimal = (int)((s.cgpa - whole) * 100 + 0.5);
    if (whole == 0) cgpaStr = "0";
    else {
        string wStr = "";
        int w = whole;
        while (w > 0) {
            wStr = char('0' + (w % 10)) + wStr;
            w /= 10;
        }
        cgpaStr = wStr;
    }
    cgpaStr += ".";
    if (decimal < 10) cgpaStr += "0";
    string dStr = "";
    int d = decimal;
    while (d > 0) {
        dStr = char('0' + (d % 10)) + dStr;
        d /= 10;
    }
    if (dStr == "") dStr = "0";
    cgpaStr += dStr;

    row.data[4] = cgpaStr;
    row.data[5] = s.status;

    appendTXT("students.txt", row, 6);
    cout << "Student added successfully!" << endl;
}

// ============================================================
// searchByRoll:  search student by roll number
// ============================================================
void searchByRoll() {
    cout << "\n========== SEARCH BY ROLL NUMBER ==========" << endl;
    cout << "Enter Roll Number: ";
    string roll;
    cin >> roll;

    CSVRow result;
    if (findRow("students.txt", roll, 0, result)) {
        cout << "\n--- Student Found ---" << endl;
        cout << "Roll No:    " << result.data[0] << endl;
        cout << "Name:       " << result.data[1] << endl;
        cout << "Department: " << result.data[2] << endl;
        cout << "Semester:   " << result.data[3] << endl;
        cout << "CGPA:       " << result.data[4] << endl;
        cout << "Status:     " << result.data[5] << endl;
    } else {
        cout << "Student not found!" << endl;
    }
}

// ============================================================
// searchByName: search names of the student
// ============================================================
void searchByName() {
    cout << "\n========== SEARCH BY NAME ==========" << endl;
    cout << "Enter Name (or part of name): ";
    string searchName;
    cin.ignore();
    getline(cin, searchName);

    // Search in lowercase  converion
    string searchLower = "";
    for (int i = 0; i < searchName.length(); i++) {
        char c = searchName[i];
        if (c >= 'A' && c <= 'Z') c = c + ('a' - 'A');
        searchLower += c;
    }

    CSVRow rows[MAX_ROWS];
    int rowCount = 0, colCount = 0;
    readTXT("students.txt", rows, rowCount, colCount);

    bool found = false;
    cout << "\n--- Matching Students ---" << endl;

    for (int i = 0; i < rowCount; i++) {
        string name = rows[i].data[1];

        // Name in lowercase converion
        string nameLower = "";
        for (int j = 0; j < name.length(); j++) {
            char c = name[j];
            if (c >= 'A' && c <= 'Z') c = c + ('a' - 'A');
            nameLower += c;
        }

        // Check search name  part
        bool isSubstring = false;
        for (int j = 0; j <= (int)(nameLower.length() - searchLower.length()); j++) {
            bool match = true;
            for (int k = 0; k < searchLower.length(); k++) {
                if (nameLower[j + k] != searchLower[k]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                isSubstring = true;
                break;
            }
        }

        if (isSubstring) {
            found = true;
            cout << "Roll: " << rows[i].data[0] << " | Name: " << rows[i].data[1] << endl;
        }
    }

    if (!found) {
        cout << "No students found with that name." << endl;
    }
}

// ============================================================
// updateStudent: Student update
// ============================================================
void updateStudent() {
    cout << "\n========== UPDATE STUDENT ==========" << endl;
    cout << "Enter Roll Number: ";
    string roll;
    cin >> roll;

    const int MAX = 100;
    Student students[MAX];
    int count = loadStudents(students, MAX);

    // Search Student
    int index = -1;
    for (int i = 0; i < count; i++) {
        if (students[i].roll_no == roll) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Student not found!" << endl;
        return;
    }

    // Current details
    cout << "\nCurrent Details:" << endl;
    cout << "1. Name:       " << students[index].name << endl;
    cout << "2. Department: " << students[index].department << endl;
    cout << "3. Semester:   " << students[index].semester << endl;
    cout << "4. CGPA:       " << students[index].cgpa << endl;
    cout << "5. Status:     " << students[index].status << endl;

    cout << "\nEnter field number to update (1-5): ";
    int choice;
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1:
            cout << "Enter new Name: ";
            getline(cin, students[index].name);
            if (hasDigits(students[index].name)) {
                cout << "Error: Name cannot contain digits!" << endl;
                return;
            }
            break;
        case 2:
            cout << "Enter new Department: ";
            getline(cin, students[index].department);
            break;
        case 3:
            cout << "Enter new Semester: ";
            {
                string semInput;
                getline(cin, semInput);
                students[index].semester = 0;
                for (int i = 0; i < semInput.length(); i++) {
                    if (semInput[i] >= '0' && semInput[i] <= '9') {
                        students[index].semester = students[index].semester * 10 + (semInput[i] - '0');
                    }
                }
            }
            break;
        case 4:
            cout << "Enter new CGPA (0.0-4.0): ";
            {
                string cgpaInput;
                getline(cin, cgpaInput);
                students[index].cgpa = 0.0;
                bool afterDecimal = false;
                double decimalPlace = 0.1;
                for (int i = 0; i < cgpaInput.length(); i++) {
                    if (cgpaInput[i] == '.') {
                        afterDecimal = true;
                    } else if (cgpaInput[i] >= '0' && cgpaInput[i] <= '9') {
                        if (!afterDecimal) {
                            students[index].cgpa = students[index].cgpa * 10 + (cgpaInput[i] - '0');
                        } else {
                            students[index].cgpa += (cgpaInput[i] - '0') * decimalPlace;
                            decimalPlace *= 0.1;
                        }
                    }
                }
                if (students[index].cgpa < 0.0 || students[index].cgpa > 4.0) {
                    cout << "Error: CGPA must be between 0.0 and 4.0!" << endl;
                    return;
                }
            }
            break;
        case 5:
            cout << "Enter new Status (active/inactive): ";
            getline(cin, students[index].status);
            break;
        default:
            cout << "Invalid choice!" << endl;
            return;
    }

    saveStudents(students, count);
    cout << "Student updated successfully!" << endl;
}

// ============================================================
// softDeleteStudent: Status inactive
// ============================================================
void softDeleteStudent() {
    cout << "\n========== SOFT DELETE STUDENT ==========" << endl;
    cout << "Enter Roll Number: ";
    string roll;
    cin >> roll;

    const int MAX = 100;
    Student students[MAX];
    int count = loadStudents(students, MAX);

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (students[i].roll_no == roll) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Student not found!" << endl;
        return;
    }

    students[index].status = "inactive";
    saveStudents(students, count);
    cout << "Student soft-deleted (status set to inactive)." << endl;
}

// ============================================================
// listActiveStudents: Active students sorted by roll (Selection Sort)
// ============================================================
void listActiveStudents() {
    const int MAX = 100;
    Student students[MAX];
    int count = loadStudents(students, MAX);

    // Sirf active students filter karo
    Student activeStudents[MAX];
    int activeCount = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].status == "active") {
            activeStudents[activeCount] = students[i];
            activeCount++;
        }
    }

    if (activeCount == 0) {
        cout << "No active students found." << endl;
        return;
    }

    // SELECTION SORT by roll number
    for (int i = 0; i < activeCount - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < activeCount; j++) {
            // String compare manually
            bool isLess = false;
            int len1 = activeStudents[j].roll_no.length();
            int len2 = activeStudents[minIdx].roll_no.length();
            int minLen = len1 < len2 ? len1 : len2;

            for (int k = 0; k < minLen; k++) {
                if (activeStudents[j].roll_no[k] < activeStudents[minIdx].roll_no[k]) {
                    isLess = true;
                    break;
                } else if (activeStudents[j].roll_no[k] > activeStudents[minIdx].roll_no[k]) {
                    break;
                }
            }

            if (isLess) {
                minIdx = j;
            }
        }

        // Swap
        Student temp = activeStudents[i];
        activeStudents[i] = activeStudents[minIdx];
        activeStudents[minIdx] = temp;
    }

    cout << "\n========== ACTIVE STUDENTS (Sorted by Roll) ==========" << endl;
    cout << "Roll No\t\tName\t\t\tCGPA\tStatus" << endl;
    cout << "--------------------------------------------------------" << endl;
    for (int i = 0; i < activeCount; i++) {
        cout << activeStudents[i].roll_no << "\t"
             << activeStudents[i].name << "\t\t"
             << activeStudents[i].cgpa << "\t"
             << activeStudents[i].status << endl;
    }
}
