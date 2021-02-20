//
// Created by lenovo on 2021/1/31.
//
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <sstream>
#include "program.h"
using namespace std;

extern std::stack<USER> users;

void initial(){
    fstream in;
    /*in.open("userid.dat" , ios::out | ios::binary);
    in.close();
    in.open("user.dat" , ios::out | ios::binary);
    in.close();
    in.open("ISBN.dat" , ios::out | ios::binary);
    in.close();
    in.open("name.dat" , ios::out | ios::binary);
    in.close();
    in.open("author.dat" , ios::out | ios::binary);
    in.close();
    in.open("keyword.dat" , ios::out | ios::binary);
    in.close();
    in.open("bookFile.dat" , ios::out | ios::binary);
    in.close();
    in.open("transcationFile.dat" , ios::out | ios::binary);
    in.close();
    in.open("financeFile.dat" , ios::out | ios::binary);
    in.seekp(0);
    Finance tmp;
    in.write(reinterpret_cast<char*>(&tmp) , sizeof(Finance));
    in.close();
    USER u("root" , "sjtu" , "root" , 7);
    useradd(u);*/
    in.open("user.dat" , ios::in);
    if (!in){
        in.open("userid.dat" , ios::out | ios::binary);
        in.close();
        in.open("user.dat" , ios::out | ios::binary);
        in.close();
        in.open("ISBN.dat" , ios::out | ios::binary);
        in.close();
        in.open("name.dat" , ios::out | ios::binary);
        in.close();
        in.open("author.dat" , ios::out | ios::binary);
        in.close();
        in.open("keyword.dat" , ios::out | ios::binary);
        in.close();
        in.open("bookFile.dat" , ios::out | ios::binary);
        in.close();
        in.open("transcationFile.dat" , ios::out | ios::binary);
        in.close();
        in.open("ReportFinance.dat" , ios::out | ios::binary);
        in.close();
        in.open("action.dat" , ios::out | ios::binary);
        in.close();
        in.open("log.dat" , ios::out | ios::binary);
        in.close();
        in.open("financeFile.dat" , ios::out | ios::binary);
        in.seekp(0);
        Finance tmp;
        in.write(reinterpret_cast<char*>(&tmp) , sizeof(Finance));
        in.close();
        USER u("root" , "sjtu" , "root" , 7);
        useradd(u);
    }
    in.close();
}

void run(const std::string& command){
    string token;
    int i = 0;
    for (; i < command.length() && command[i] != ' '; ++i) token += command[i];
    if (token == "su"){
        string userid , password;
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) userid += command[i];
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) password += command[i];
        login(userid , password);
        return;
    }
    else if (token == "logout"){
        if (users.empty()) throw "error";
        logout();
        return;
    }
    else if (token == "useradd"){
        if (users.empty() || users.top().privilege < 3) throw "error";
        string userid , password , username;
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) userid += command[i];
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) password += command[i];
        ++i;
        int _privilege = command[i] - '0';
        ++i;
        ++i;
        username = command.substr(i);
        //for (; i < command.length() && command[i] != ' '; ++i) username += command[i];
        if (users.top().privilege <= _privilege || _privilege == 7) throw "error";
        //if (username.find(' ')) throw "error";
        for (char j : username){
            if (j == ' ') throw "error";
        }
        USER o(userid , password , username , _privilege);
        useradd(o);
        return;
    }
    else if (token == "register"){
        string userid , password , username;
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) userid += command[i];
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) password += command[i];
        ++i;
        username = command.substr(i);
        //for (; i < command.length() && command[i] != ' '; ++i) username += command[i];
        //if (username.find(' ')) throw "error";
        for (int j = 0 ; j < username.length() ; ++j){
            if (username[j] == ' ') throw "error";
        }
        USER o(userid , password , username , 1);
        Register(o);
        return;
    }
    else if (token == "delete"){
        if (users.empty() || users.top().privilege != 7) throw "error";
        string userid;
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) userid += command[i];
        if (userid == "root") throw "error";
        Delete(userid.c_str());
    }
    else if (token == "passwd"){
        if (users.empty()) throw "error";
        string userid , oldPass , newPass;
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) userid += command[i];
        if (users.top().privilege == 7) {
            ++i;
            for (; i < command.length() && command[i] != ' '; ++i) newPass += command[i];
            passwordChange(userid.c_str() , "" , newPass.c_str());
        }
        else {
            ++i;
            for (; i < command.length() && command[i] != ' '; ++i) oldPass += command[i];
            ++i;
            for (; i < command.length() && command[i] != ' '; ++i) newPass += command[i];
            if (oldPass.empty() || newPass.empty() /*|| oldPass == newPass*/) throw "error";
            passwordChange(userid.c_str() , oldPass.c_str() , newPass.c_str());
        }
    }
    else if (token == "select"){
        if (users.empty() || users.top().privilege < 3) throw "error";
        string isbn;
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) isbn += command[i];
        select(isbn.c_str());
        return;
    }
    else if (token == "modify"){
        if (users.empty() || users.top().select == -1 || users.top().privilege < 3) throw "error";
        while (i < command.length()){
            ++i;
            string t;
            for (; i < command.length() && command[i] != ' ' ; ++i) t += command[i];
            fstream r;
            r.open("log.dat" , ios::out | ios::binary);
            r.seekp(0 , ios::end);
            LOG tmp;
            strcpy(tmp.username , users.top().username);
            string x = "modify " + t;
            strcpy(tmp.action , x.c_str());
            string type = t.substr(1 , t.find('=') - 1);
            if (type == "ISBN") {
                string isbn = t.substr(6);
                modifyISBN(isbn.c_str());
            }
            else if (type == "name") {
                t = t.substr(6);
                string name = t.substr(1 , t.length() - 2);
                modifyName(name.c_str());
            }
            else if (type == "author") {
                t = t.substr(8);
                string author = t.substr(1 , t.length() - 2);
                modifyAuthor(author.c_str());
            }
            else if (type == "keyword") {
                t = t.substr(9);
                string keyword = t.substr(1 , t.length() - 2);
                modifyKeyword(keyword.c_str());
            }
            else if (type == "price") {
                t = t.substr(7);
                double price;
                stringstream w;
                w << t;
                w >> price;
                /*string a1 = t.substr(0 , t.find('.'));
                for (int j = 0 ; j < a1.length() ; ++j){
                    price += (a1[j] - '0') * pow(10 , a1.length() - j - 1);
                }
                if (t.find('.') != string::npos) {
                    string a2 = t.substr(t.find('.') + 1);
                    for (int j = 0 ; j < a1.length() ; ++j){
                        price += (a1[j] - '0') * pow(10 , j - 1);
                    }
                }*/
                modifyPrice(price);
            }
        }
        return;
    }
    else if (token == "import"){
        if (users.empty() || users.top().select == -1 || users.top().privilege < 3) throw "error";
        string t1 , t2;
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) {
            if (command[i] < '0' || command[i] > '9') throw "error";
            t1 += command[i];
        }
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) t2 += command[i];
        int quantity = 0;
        for (int j = 0 ; j < t1.length() ; ++j){
            quantity += (t1[j] - '0') * pow(10 , t1.length() - j - 1);
        }
        double price;
        stringstream w;
        w << t2;
        w >> price;
        import(quantity , price);
    }
    else if (token == "show"){
        string t;
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) t += command[i];
        if (t.length() == 0) {
            BOOK tmp;
            vector <BOOK> b;
            fstream r;
            r.open("bookFile.dat" , ios::in | ios::out |ios::binary);
            r.seekg(0 , ios::end);
            int p = r.tellg();
            int j = 0;
            while (j < p){
                r.seekg(j);
                r.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
                b.push_back(tmp);
                j += sizeof(BOOK);
            }
            sort(b.begin() , b.end());
            for (auto it : b) it.show();
            r.close();
        }
        else if (t == "finance"){
            if (users.empty() || users.top().privilege < 7 ) throw "error";
            string t = "";
            ++i;
            for (; i < command.length() && command[i] != ' '; ++i) t += command[i];
            if (!t.empty()) {
                int time;
                stringstream w;
                w << t;
                w >> time;
                if (time != 0) {
                    showFinance_t(time);
                }
            }
            else showFinance();
            /*string tt;
            ++i;
            for (; i < command.length() && command[i] != ' '; ++i) tt += command[i];

            else showFinance();*/
        }
        else {
            if (users.empty()) throw "error";
            string type = t.substr(1 , t.find('=') - 1);
            if (type == "ISBN") {
                string isbn = t.substr(6);
                showISBN(isbn);
            }
            else if (type == "name") {
                t = t.substr(6);
                string name = t.substr(1 , t.length() - 2);
                showName(name);
            }
            else if (type == "author") {
                t = t.substr(8);
                string author = t.substr(1 , t.length() - 2);
                showAuthor(author);
            }
            else if (type == "keyword") {
                t = t.substr(9);
                string keyword = t.substr(1 , t.length() - 2);
                showKeyword(keyword);
            }
        }
        return;
    }//two type
    else if (token == "buy"){
        if (users.empty()) throw "error";
        string isbn , t;
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) isbn += command[i];
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) t += command[i];
        int quantity = 0;
        for (int j = 0 ; j < t.length() ; ++j){
            quantity += (t[j] - '0') * pow(10 , t.length() - j - 1);
        }
        buy(isbn.c_str() , quantity);
        return;
    }
    else if (token == "report"){
        if (users.empty()) throw "error";
        string type;
        ++i;
        for (; i < command.length() && command[i] != ' '; ++i) type += command[i];
        if (type == "finance"){
            if (users.top().privilege != 7) throw "error";
            reportFinance();
        }
        if (type == "employee"){
            if (users.top().privilege != 7) throw "error";
            reportEmployee();
        }
        if (type == "myself"){
            if (users.top().privilege < 3) throw "error";
            reportMyself(users.top().userid);
        }
    }//three type
    else if (token == "log"){
        if (users.empty() || users.top().privilege != 7) throw "error";
        log();
    }
    else if (token == "quit"){
        quit();
    }
    else if (token == "exit"){
        exit();
    }
    else if (token == "debug"){
        debug();
    }
    else throw "error";
}

void exit(){
    exit(0);
};

void quit(){
    exit(0);
};

void debug(){
    Block tmp1;
    fstream r;
    r.open("keyword.dat" , ios::in | ios::out | ios::binary);
    r.seekg(0);
    r.read(reinterpret_cast<char*>(&tmp1) , sizeof(Block));
    for (int i = 0 ; i < 43 ; ++i){
        r.seekg(tmp1.next);
        r.read(reinterpret_cast<char*>(&tmp1) , sizeof(Block));
    }
    for (int i = 0 ; i < 15 ; ++i){
        r.seekg(tmp1.next);
        r.read(reinterpret_cast<char*>(&tmp1) , sizeof(Block));
    }
    tmp1.nodes->showKey();
}