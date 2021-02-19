//
// Created by lenovo on 2021/2/4.
//

#include <fstream>
#include <iostream>

#ifndef BOOKSTORE_STORE_H
#define BOOKSTORE_STORE_H

class Finance{
public:
    double benefit = 0;
    double expense = 0;
    Finance();
};

class Transcation{
public:
    char ISBN[60] = "";
    int quantity = 0;
    double cost = 0;
    Transcation();
};

void showFinance();

void showFinance_t(int time);

void reportFinance();

void reportEmployee();

void log();

void reportMyself();

#endif //BOOKSTORE_STORE_H
