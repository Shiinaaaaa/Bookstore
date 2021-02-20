//
// Created by lenovo on 2021/2/4.
//

#include "store.h"
#include <cstring>

using namespace std;

Finance::Finance() = default;

Transcation::Transcation() = default;

ReportFinance::ReportFinance() = default;

Action::Action() = default;

LOG::LOG() = default;

void showFinance(){
    fstream r;
    r.open("financeFile.dat" , ios::in | ios::out |ios::binary);
    if (!r) throw "open fail";
    r.seekg(0);
    Finance tmp;
    r.read(reinterpret_cast<char*>(&tmp) , sizeof(Finance));
    cout << "+ ";
    printf("%.2lf" , tmp.benefit);
    cout << " - ";
    printf("%.2lf" , tmp.expense);
    cout << endl;
    r.close();
}

void showFinance_t(int time){
    fstream r;
    r.open("transcationFile.dat" , ios::in | ios::out |ios::binary);
    if (!r) throw "open error";
    r.seekg(0 , ios::end);
    int p = r.tellg();
    int size = sizeof(Transcation);
    int t = p - ( size * time );
    if (t < 0) throw "error";
    p -= sizeof(Transcation);
    r.seekg(p);
    Transcation tmp;
    int i = 0;
    double totalBenifit = 0 , totalExpense = 0;
    for (; i < time ; ++i){
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(Transcation));
        if (tmp.cost > 0) totalBenifit += tmp.cost;
        else totalExpense -= tmp.cost;
        p -= sizeof(Transcation);
        r.seekg(p);
    }
    cout << "+ ";
    printf("%.2lf" , totalBenifit);
    cout << " - ";
    printf("%.2lf" , totalExpense);
    cout << endl;
    r.close();
}

void reportFinance(){
    showFinance();
    fstream r;
    r.open("ReportFinance.dat" , ios::in | ios::out | ios::binary);
    r.seekg(0);
    while(!r.eof()){
        ReportFinance tmp;
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(ReportFinance));
        std::cout << tmp.Do << " " << tmp.money << endl;
    }
    r.close();
};

void reportEmployee(){
    fstream r;
    r.open("action.dat" , ios::in | ios::out | ios::binary);
    r.seekg(0);
    while(!r.eof()){
        Action tmp;
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(Action));
        std::cout << tmp.name << "\t" << tmp.action << endl;
    }
    r.close();
};

void reportMyself(char *_userid){
    fstream r;
    r.open("action.dat" , ios::in | ios::out | ios::binary);
    r.seekg(0);
    while(!r.eof()){
        Action tmp;
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(Action));
        if (strcmp(_userid , tmp.name) == 0) {
            std::cout << tmp.name << "\t" << tmp.action << endl;
        }
    }
    r.close();
};

void log(){
    fstream r;
    r.open("log.dat" , ios::in | ios::out | ios::binary);
    r.seekg(0);
    while(!r.eof()){
        LOG tmp;
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(LOG));
        if (tmp.money == -1) std::cout << tmp.username << "\t" << tmp.action << endl;
        else std::cout << tmp.username << "\t" << tmp.action << tmp.money << endl;
    }
    r.close();
};