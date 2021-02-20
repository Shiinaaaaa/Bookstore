//
// Created by lenovo on 2021/2/4.
//

#include "user.h"
#include "store.h"
#include "Blocklist.h"
#include <iostream>

std::stack<USER> users;

BlockList useridList("userid.dat");
//BlockList userList("user.dat");

USER::USER(const string& _userid, const string& _password, const string& _username, int _privilege) {
    strcpy(userid , _userid.c_str());
    strcpy(password , _password.c_str());
    strcpy(username , _username.c_str());
    privilege = _privilege;
}

USER::USER() = default;

void login(const string& _userid , const string& _password){
    if (!useridList.existNode(_userid)) throw "nofind";
    USER o;
    fstream r;
    r.open("user.dat" , ios::in | ios::out |ios::binary);
    if (!r) throw "open fail";
    r.seekg(useridList.FindNode(_userid));
    r.read(reinterpret_cast<char*>(&o) , sizeof(USER));
    if (_password.length() == 0){
        if (users.empty()) throw "error";
        if (o.privilege < users.top().privilege) users.push(o);
        else throw "cannot log in";
    }
    else {
        if (strcmp(o.password , _password.c_str()) == 0) users.push(o);
        else throw "wrong password";
    }
    r.close();
    r.open("action.dat" , ios::in | ios::out |ios::binary);
    r.seekp(0 , ios::end);
    Action tmp;
    strcpy(tmp.action , "login");
    strcpy(tmp.name , o.username);
    r.write(reinterpret_cast<char*>(&tmp) , sizeof(Action));
    r.close();
    r.open("log.dat" , ios::in | ios::out |ios::binary);
    r.seekp(0 , ios::end);
    LOG tmp2;
    strcpy(tmp2.action , "login");
    strcpy(tmp2.username , o.username);
    r.write(reinterpret_cast<char*>(&tmp2) , sizeof(LOG));
    r.close();
};

void logout(){
    if (users.empty()) throw "empty";
    //users.top().select = -1;
    fstream r;
    r.open("action.dat" , ios::in | ios::out |ios::binary);
    r.seekp(0 , ios::end);
    Action tmp;
    strcpy(tmp.action , "logout");
    strcpy(tmp.name , users.top().username);
    r.write(reinterpret_cast<char*>(&tmp) , sizeof(Action));
    r.close();
    r.open("log.dat" , ios::in | ios::out |ios::binary);
    r.seekp(0 , ios::end);
    LOG tmp2;
    strcpy(tmp2.action , "logout");
    strcpy(tmp2.username , users.top().username);
    r.write(reinterpret_cast<char*>(&tmp2) , sizeof(LOG));
    r.close();
    users.pop();
};

void useradd(USER &o){
    if (useridList.existNode(o.userid)) {throw "existed" ;}
    fstream w;
    w.open("user.dat" , ios::in | ios::out |ios::binary);
    if (!w) throw ("open fail");
    w.seekp(0 , ios::end);
    int p = w.tellp();
    w.write(reinterpret_cast<char*>(&o) , sizeof(USER));
    Node a(o.userid , p);
    useridList.AddNode(a);
    if (strcmp(o.userid , "root") != 0){
        fstream r;
        r.open("action.dat" , ios::in | ios::out |ios::binary);
        r.seekp(0 , ios::end);
        Action tmp;
        string n = o.username;
        string x = "add user " + n;
        strcpy(tmp.action , x.c_str());
        strcpy(tmp.name , users.top().username);
        r.write(reinterpret_cast<char*>(&tmp) , sizeof(Action));
        r.close();
        r.open("log.dat" , ios::in | ios::out |ios::binary);
        r.seekp(0 , ios::end);
        LOG tmp2;
        strcpy(tmp2.action , x.c_str());
        strcpy(tmp2.username , users.top().username);
        r.write(reinterpret_cast<char*>(&tmp2) , sizeof(LOG));
        r.close();
    }
};

void Register(USER &o){
    if (useridList.existNode(o.userid)) {throw "existed" ;}
    fstream w;
    w.open("user.dat" , ios::in | ios::out |ios::binary);
    if (!w) throw "open fail";
    w.seekp(0 , ios::end);
    int p = w.tellp();
    w.write(reinterpret_cast<char*>(&o) , sizeof(USER));
    w.close();
    Node a(o.userid , p);
    useridList.AddNode(a);
    fstream r;
    r.open("action.dat" , ios::in | ios::out |ios::binary);
    r.seekp(0 , ios::end);
    Action tmp;
    string n = o.username;
    strcpy(tmp.action , "register");
    strcpy(tmp.name , n.c_str());
    r.write(reinterpret_cast<char*>(&tmp) , sizeof(Action));
    r.close();
    r.open("log.dat" , ios::in | ios::out |ios::binary);
    r.seekp(0 , ios::end);
    LOG tmp2;
    strcpy(tmp2.action , "register");
    strcpy(tmp2.username , n.c_str());
    r.write(reinterpret_cast<char*>(&tmp2) , sizeof(LOG));
    r.close();
};

void Delete(const char _userid[]){
    if (!useridList.existNode(_userid))  {throw "existed" ;}
    Node o(_userid);
    useridList.DeleteNode(o);
    fstream r;
    r.open("action.dat" , ios::in | ios::out |ios::binary);
    r.seekp(0 , ios::end);
    Action tmp;
    strcpy(tmp.action , "delete");
    strcpy(tmp.name , _userid);
    r.write(reinterpret_cast<char*>(&tmp) , sizeof(Action));
    r.close();
    r.open("log.dat" , ios::in | ios::out |ios::binary);
    r.seekp(0 , ios::end);
    LOG tmp2;
    strcpy(tmp2.action , "delete");
    strcpy(tmp2.username , _userid);
    r.write(reinterpret_cast<char*>(&tmp2) , sizeof(LOG));
    r.close();
};

void passwordChange(const char _userid[] , const char password[] , const char newPassword[]){
    if (!useridList.existNode(_userid)) throw "nofind";
    USER o;
    fstream r;
    r.open("user.dat" , ios::in | ios::out |ios::binary);
    if (!r) throw "open fail";
    r.seekg(useridList.FindNode(_userid));
    int position = r.tellg();
    r.read(reinterpret_cast<char*>(&o) , sizeof(USER));
    if (users.top().privilege == 7 || strcmp(o.password , password) == 0) {
        strcpy(o.password , newPassword);
    }
    else throw "wrong password";
    fstream w;
    w.open("user.dat" , ios::in | ios::out |ios::binary);
    if (!w) throw "open fail";
    w.seekp(position);
    w.write(reinterpret_cast<char*>(&o) , sizeof(USER));
    r.close();
    w.close();
    r.open("action.dat" , ios::in | ios::out |ios::binary);
    r.seekp(0 , ios::end);
    Action tmp;
    strcpy(tmp.action , "change password");
    strcpy(tmp.name , _userid);
    r.write(reinterpret_cast<char*>(&tmp) , sizeof(Action));
    r.close();
    r.open("log.dat" , ios::in | ios::out |ios::binary);
    r.seekp(0 , ios::end);
    LOG tmp2;
    strcpy(tmp2.action , "change password");
    strcpy(tmp2.username , _userid);
    r.write(reinterpret_cast<char*>(&tmp2) , sizeof(LOG));
    r.close();
};
