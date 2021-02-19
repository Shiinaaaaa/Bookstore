//
// Created by lenovo on 2021/2/4.
//

#include "store.h"

using namespace std;

Finance::Finance() = default;

Transcation::Transcation() = default;

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


void reportFinance(){};

void reportEmployee(){};

void log(){};

void reportMyself(){};