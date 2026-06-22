#include "fee_tracker.h"

int daysBetween(const string& date1, const string& date2) {
    int d1 = (date1[0] - '0') * 10 + (date1[1] - '0');
    int m1 = (date1[3] - '0') * 10 + (date1[4] - '0');
    int y1 = (date1[6] - '0') * 1000 + (date1[7] - '0') * 100 + 
             (date1[8] - '0') * 10 + (date1[9] - '0');

    int d2 = (date2[0] - '0') * 10 + (date2[1] - '0');
    int m2 = (date2[3] - '0') * 10 + (date2[4] - '0');
    int y2 = (date2[6] - '0') * 1000 + (date2[7] - '0') * 100 + 
             (date2[8] - '0') * 10 + (date2[9] - '0');

    int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    auto totalDays = [&](int d, int m, int y) -> int {
        int total = 0;

        for (int year = 0; year < y; year++) {
            bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            total += leap ? 366 : 365;
        }

        for (int month = 1; month < m; month++) {
            int days = monthDays[month - 1];
            if (month == 2) {
                bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
                if (leap) days = 29;
            }
            total += days;
        }

        total += d;
        return total;
    };

    int days1 = totalDays(d1, m1, y1);
    int days2 = totalDays(d2, m2, y2);

    return days2 - days1;
}

double computeLateFine(const string& feeId) {
    vector<vector<string>> fees = readTXT("fees.txt");

    for (int i = 1; i < fees.size(); i++) {
        if (fees[i].size() < 9) continue;
        if (fees[i][0] == feeId) {
            string dueDate = fees[i][5];
            string paidDate = fees[i][6];
            string totalFeeStr = fees[i][3];

            double totalFee = 0.0;
            bool afterDecimal = false;
            double decimalPlace = 0.1;
            for (int j = 0; j < totalFeeStr.length(); j++) {
                if (totalFeeStr[j] == '.') {
                    afterDecimal = true;
                } else if (totalFeeStr[j] >= '0' && totalFeeStr[j] <= '9') {
                    if (!afterDecimal) {
                        totalFee = totalFee * 10 + (totalFeeStr[j] - '0');
                    } else {
                        totalFee += (totalFeeStr[j] - '0') * decimalPlace;
                        decimalPlace *= 0.1;
                    }
                }
            }

            if (paidDate == "00-00-0000") {
                return 0.0;
            }

            int days = daysBetween(dueDate, paidDate);
            if (days <= 0) return 0.0;

            int completeWeeks = days / 7;
            double fine = totalFee * 0.02 * completeWeeks;

            return fine;
        }
    }

    return 0.0;
}

void recordPayment() {
    cout << "\n========== RECORD PAYMENT ==========" << endl;

    cout << "Enter Fee ID (e.g., F001): ";
    string feeId;
    cin >> feeId;

    vector<vector<string>> fees = readTXT("fees.txt");
    int index = -1;

    for (int i = 1; i < fees.size(); i++) {
        if (fees[i].size() > 0 && fees[i][0] == feeId) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Fee record not found!" << endl;
        return;
    }

    cout << "Current Paid Amount: " << fees[index][4] << endl;
    cout << "Total Fee: " << fees[index][3] << endl;

    cout << "Enter Payment Amount: ";
    string amountStr;
    cin >> amountStr;

    double currentPaid = 0.0;
    bool afterDecimal = false;
    double decimalPlace = 0.1;
    for (int j = 0; j < fees[index][4].length(); j++) {
        if (fees[index][4][j] == '.') {
            afterDecimal = true;
        } else if (fees[index][4][j] >= '0' && fees[index][4][j] <= '9') {
            if (!afterDecimal) {
                currentPaid = currentPaid * 10 + (fees[index][4][j] - '0');
            } else {
                currentPaid += (fees[index][4][j] - '0') * decimalPlace;
                decimalPlace *= 0.1;
            }
        }
    }

    double newAmount = 0.0;
    afterDecimal = false;
    decimalPlace = 0.1;
    for (int j = 0; j < amountStr.length(); j++) {
        if (amountStr[j] == '.') {
            afterDecimal = true;
        } else if (amountStr[j] >= '0' && amountStr[j] <= '9') {
            if (!afterDecimal) {
                newAmount = newAmount * 10 + (amountStr[j] - '0');
            } else {
                newAmount += (amountStr[j] - '0') * decimalPlace;
                decimalPlace *= 0.1;
            }
        }
    }

    double totalPaid = currentPaid + newAmount;

    string paidStr = "";
    int whole = (int)totalPaid;
    int decimal = (int)((totalPaid - whole) * 100 + 0.5);
    if (whole == 0) paidStr = "0";
    else {
        string w = "";
        int t = whole;
        while (t > 0) {
            w = char('0' + (t % 10)) + w;
            t /= 10;
        }
        paidStr = w;
    }
    paidStr += ".";
    if (decimal < 10) paidStr += "0";
    string d = "";
    int tt = decimal;
    while (tt > 0) {
        d = char('0' + (tt % 10)) + d;
        tt /= 10;
    }
    if (d == "") d = "0";
    paidStr += d;

    fees[index][4] = paidStr;

    cout << "Enter Payment Date (DD-MM-YYYY): ";
    string payDate;
    cin >> payDate;
    fees[index][6] = payDate;

    cout << "Enter Payment Method (Online/Cash/Bank): ";
    string method;
    cin >> method;
    fees[index][7] = method;

    double totalFee = 0.0;
    afterDecimal = false;
    decimalPlace = 0.1;
    for (int j = 0; j < fees[index][3].length(); j++) {
        if (fees[index][3][j] == '.') {
            afterDecimal = true;
        } else if (fees[index][3][j] >= '0' && fees[index][3][j] <= '9') {
            if (!afterDecimal) {
                totalFee = totalFee * 10 + (fees[index][3][j] - '0');
            } else {
                totalFee += (fees[index][3][j] - '0') * decimalPlace;
                decimalPlace *= 0.1;
            }
        }
    }

    if (totalPaid >= totalFee) {
        int days = daysBetween(fees[index][5], payDate);
        if (days > 0) {
            fees[index][8] = "paid_late";
        } else {
            fees[index][8] = "paid";
        }
    } else if (totalPaid > 0) {
        fees[index][8] = "partial";
    }

    writeTXT("fees.txt", fees);
    cout << "Payment recorded successfully!" << endl;
}

void generateReceipt() {
    cout << "\n========== GENERATE RECEIPT ==========" << endl;

    cout << "Enter Fee ID: ";
    string feeId;
    cin >> feeId;

    vector<vector<string>> fees = readTXT("fees.txt");
    int index = -1;

    for (int i = 1; i < fees.size(); i++) {
        if (fees[i].size() > 0 && fees[i][0] == feeId) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Fee record not found!" << endl;
        return;
    }

    double totalFee = 0.0, paid = 0.0;
    bool afterDecimal = false;
    double decimalPlace = 0.1;

    for (int j = 0; j < fees[index][3].length(); j++) {
        if (fees[index][3][j] == '.') {
            afterDecimal = true;
        } else if (fees[index][3][j] >= '0' && fees[index][3][j] <= '9') {
            if (!afterDecimal) {
                totalFee = totalFee * 10 + (fees[index][3][j] - '0');
            } else {
                totalFee += (fees[index][3][j] - '0') * decimalPlace;
                decimalPlace *= 0.1;
            }
        }
    }

    afterDecimal = false;
    decimalPlace = 0.1;
    for (int j = 0; j < fees[index][4].length(); j++) {
        if (fees[index][4][j] == '.') {
            afterDecimal = true;
        } else if (fees[index][4][j] >= '0' && fees[index][4][j] <= '9') {
            if (!afterDecimal) {
                paid = paid * 10 + (fees[index][4][j] - '0');
            } else {
                paid += (fees[index][4][j] - '0') * decimalPlace;
                decimalPlace *= 0.1;
            }
        }
    }

    double fine = computeLateFine(feeId);
    double balance = totalFee + fine - paid;

    vector<string> student = findRow("students.txt", 0, fees[index][1]);
    string name = (student.size() > 1) ? student[1] : "Unknown";

    cout << "\n" << setfill('=') << setw(50) << "" << endl;
    cout << setfill(' ') << setw(20) << "" << "FEE RECEIPT" << endl;
    cout << setfill('=') << setw(50) << "" << endl;
    cout << setfill(' ');
    cout << left << setw(20) << "Fee ID: " << feeId << endl;
    cout << left << setw(20) << "Roll Number: " << fees[index][1] << endl;
    cout << left << setw(20) << "Student Name: " << name << endl;
    cout << left << setw(20) << "Semester: " << fees[index][2] << endl;
    cout << "----------------------------------------------" << endl;
    cout << left << setw(25) << "Tuition Fee: " << "Rs. " << totalFee << endl;
    cout << left << setw(25) << "Late Fine: " << "Rs. " << fine << endl;
    cout << left << setw(25) << "Total Due: " << "Rs. " << (totalFee + fine) << endl;
    cout << "----------------------------------------------" << endl;
    cout << left << setw(25) << "Amount Paid: " << "Rs. " << paid << endl;
    cout << left << setw(25) << "Balance: " << "Rs. " << balance << endl;
    cout << "----------------------------------------------" << endl;
    cout << left << setw(25) << "Due Date: " << fees[index][5] << endl;
    cout << left << setw(25) << "Payment Date: " << fees[index][6] << endl;
    cout << left << setw(25) << "Payment Method: " << fees[index][7] << endl;
    cout << left << setw(25) << "Status: " << fees[index][8] << endl;
    cout << setfill('=') << setw(50) << "" << endl;
}

void getDefaulters() {
    cout << "\n========== FEE DEFAULTERS ==========" << endl;

    vector<vector<string>> fees = readTXT("fees.txt");
    FeeDefaulter defaulters[50];
    int defCount = 0;

    for (int i = 1; i < fees.size() && defCount < 50; i++) {
        if (fees[i].size() < 9) continue;

        double totalFee = 0.0, paid = 0.0;
        bool afterDecimal = false;
        double decimalPlace = 0.1;

        for (int j = 0; j < fees[i][3].length(); j++) {
            if (fees[i][3][j] == '.') {
                afterDecimal = true;
            } else if (fees[i][3][j] >= '0' && fees[i][3][j] <= '9') {
                if (!afterDecimal) {
                    totalFee = totalFee * 10 + (fees[i][3][j] - '0');
                } else {
                    totalFee += (fees[i][3][j] - '0') * decimalPlace;
                    decimalPlace *= 0.1;
                }
            }
        }

        afterDecimal = false;
        decimalPlace = 0.1;
        for (int j = 0; j < fees[i][4].length(); j++) {
            if (fees[i][4][j] == '.') {
                afterDecimal = true;
            } else if (fees[i][4][j] >= '0' && fees[i][4][j] <= '9') {
                if (!afterDecimal) {
                    paid = paid * 10 + (fees[i][4][j] - '0');
                } else {
                    paid += (fees[i][4][j] - '0') * decimalPlace;
                    decimalPlace *= 0.1;
                }
            }
        }

        double outstanding = totalFee - paid;

        if (outstanding > 0 && fees[i][8] != "paid" && fees[i][8] != "paid_late") {
            defaulters[defCount].roll_no = fees[i][1];

            vector<string> student = findRow("students.txt", 0, fees[i][1]);
            defaulters[defCount].name = (student.size() > 1) ? student[1] : "Unknown";

            defaulters[defCount].outstanding = outstanding;

            string dueDate = fees[i][5];
            int days = daysBetween(dueDate, "25-06-2026");
            defaulters[defCount].weeksOverdue = days / 7;
            if (defaulters[defCount].weeksOverdue < 0) defaulters[defCount].weeksOverdue = 0;

            defCount++;
        }
    }

    if (defCount == 0) {
        cout << "No fee defaulters found!" << endl;
        return;
    }

    for (int i = 0; i < defCount - 1; i++) {
        for (int j = 0; j < defCount - i - 1; j++) {
            if (defaulters[j].outstanding < defaulters[j + 1].outstanding) {
                FeeDefaulter temp = defaulters[j];
                defaulters[j] = defaulters[j + 1];
                defaulters[j + 1] = temp;
            }
        }
    }

    cout << "Roll No\t\tName\t\t\tOutstanding\tWeeks Overdue" << endl;
    cout << "----------------------------------------------------------------" << endl;
    for (int i = 0; i < defCount; i++) {
        cout << defaulters[i].roll_no << "\t\t" << defaulters[i].name << "\t\t"
             << "Rs. " << defaulters[i].outstanding << "\t\t" << defaulters[i].weeksOverdue << endl;
    }
}

void recordPaymentMenu() {
    recordPayment();
}

void generateReceiptMenu() {
    generateReceipt();
}

void getDefaultersMenu() {
    getDefaulters();
}