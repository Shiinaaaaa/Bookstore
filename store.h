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

class ReportFinance{
public:
    double money = 0;
    char Do[200] = "";
    ReportFinance();
};

class Action{
public:
    char action[200] = "";
    char name[50] = "";
    Action();
};

class LOG{
public:
    char action[200] = "";
    char username[100] = "";
    double money = -1;
    LOG();
};

void showFinance();

void showFinance_t(int time);

void reportFinance();

void reportEmployee();

void log();

void reportMyself(char *_userid);

#endif //BOOKSTORE_STORE_H
