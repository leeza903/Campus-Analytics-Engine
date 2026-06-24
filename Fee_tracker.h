#ifndef FEE_TRACKER_H
#define FEE_TRACKER_H

#include <iostream>
#include <string>
#include <iomanip>
#include "filehandler.h"
using namespace std;

struct FeeDefaulter {
    string roll_no;
    string name;
    double outstanding;
    int weeksOverdue;
};

void recordPayment();
double computeLateFine(const string& feeId);
int daysBetween(const string& date1, const string& date2);
void generateReceipt();
void getDefaulters();

void recordPaymentMenu();
void generateReceiptMenu();
void getDefaultersMenu();

#endif