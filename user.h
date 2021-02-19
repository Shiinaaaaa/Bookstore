//
// Created by lenovo on 2021/2/4.
//
#include <string>
#include <stack>
using namespace std;

#ifndef BOOKSTORE_USER_H
#define BOOKSTORE_USER_H

class USER{
public:
    char password[31] = "";
    char username[31] = "";
    int privilege = -1;
    char userid[31] = "";
    int select = -1;
    USER();
    USER(const string& _userid, const string& _password, const string& _name, int _privilege);
};


void login(const string& _userid , const string& _password);

void logout();

void useradd(USER &o);//只能添加1的user

void Register(USER &o);

void Delete(const char _userid[]);

void passwordChange(const char _userid[] , const char password[] , const char newPassword[]);

#endif //BOOKSTORE_USER_H
