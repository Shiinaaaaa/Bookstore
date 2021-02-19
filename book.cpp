//
// Created by lenovo on 2021/1/31.
//

#include "book.h"
#include "user.h"
#include "store.h"
#include <cstring>
#include <iostream>

extern std::stack<USER> users;
BlockList isbnList("ISBN.dat");
BlockList nameList("name.dat");
BlockList authorList("author.dat");
BlockList keywordList("keyword.dat");

BOOK::BOOK(const string& _isbn, const string& _name, const string& _author, const string& _keyword, double _price, int _quantity) {
    strcpy (ISBN , _isbn.c_str());
    strcpy (name , _name.c_str());
    strcpy (author , _author.c_str());
    strcpy (keyword , _keyword.c_str());
    price = _price;
    quantity = _quantity;
}

void BOOK::show() const {
    std::cout << ISBN << "\t" << name << "\t" << author << "\t" << keyword << "\t";
    printf("%.2lf\t%d\n", price , quantity);

}

void select(const char isbn[]){
    if (!isbnList.existNode(isbn)) {
        BOOK tmp(isbn , "" , "" , "" , 0.00 , 0);
        fstream w;
        if (!w) throw "open fail";
        w.open("bookFile.dat" , ios::in | ios::out |ios::binary);
        w.seekp(0 , ios::end);
        int position = w.tellp();
        w.write(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
        Node n(isbn , position);
        isbnList.AddNode(n);
        users.top().select = position;
    }
    else users.top().select = isbnList.FindNode(isbn);
};

void modifyISBN(const char _ISBN[]){
    int offset = users.top().select;
    fstream w;
    w.open("bookFile.dat" , ios::in | ios::out |ios::binary);
    w.seekg(offset);
    BOOK tmp;
    w.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    if (strcmp(tmp.ISBN , _ISBN) == 0 || isbnList.existNode(_ISBN)) throw "error";
    Node a1(tmp.ISBN , offset);
    isbnList.DeleteNode(a1);
    Node a2(_ISBN , offset);
    isbnList.AddNode(a2);
    strcpy(tmp.ISBN , _ISBN);
    w.seekp(offset);
    w.write(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    w.close();
};

void modifyName(const char _name[]){
    int offset = users.top().select;
    fstream w;
    w.open("bookFile.dat" , ios::in | ios::out |ios::binary);
    w.seekg(offset);
    BOOK tmp;
    w.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    if (strcmp(tmp.name , _name) == 0) return;
    if (nameList.existNode(tmp.name)){
        Node a1(tmp.name , offset);
        nameList.DeleteNode(a1);
    }
    Node a2(_name , offset);
    nameList.AddNode(a2);
    strcpy(tmp.name , _name);
    w.seekp(offset);
    w.write(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    w.close();
};

void modifyAuthor(const char _author[]){
    int offset = users.top().select;
    fstream w;
    w.open("bookFile.dat" , ios::in | ios::out |ios::binary);
    w.seekg(offset);
    BOOK tmp;
    w.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    if (strcmp(tmp.author , _author) == 0) return;
    if (authorList.existNode(tmp.author)){
        Node a1(tmp.author , offset);
        authorList.DeleteNode(a1);
    }
    Node a2(_author , offset);
    authorList.AddNode(a2);
    strcpy(tmp.author , _author);
    w.seekp(offset);
    w.write(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    w.close();
};

void modifyKeyword(const char _keyword[]){
    int offset = users.top().select;
    fstream w;
    w.open("bookFile.dat" , ios::in | ios::out |ios::binary);
    w.seekg(offset);
    BOOK tmp;
    w.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    if (strcmp(tmp.keyword , _keyword) == 0) return;
    vector<string> c;
    int x = 0;
    while (x < strlen(_keyword)){
        string key;
        for (; x < strlen(_keyword) && _keyword[x] != '|' ; ++x) {
            key += _keyword[x];
        }
        if (_keyword[x] == '|') ++x;
        for (const string& ii : c){
            if (ii == key) throw "error";
        }
        c.push_back(key);
    }
    int i = 0;
    int l = strlen(tmp.keyword);
    while (i < l){
        if (tmp.keyword[i] == '|') ++i;
        string key;
        for (; i < strlen(tmp.keyword) && tmp.keyword[i] != '|' ; ++i) key += tmp.keyword[i];
        if (keywordList.existNode(key)){
            Node a1(key , offset);
            keywordList.DeleteNode(a1);
        }
    }
    i = 0;
    int len = strlen(_keyword);
    while (i < len){
        string key;
        for (; i < strlen(_keyword) && _keyword[i] != '|' ; ++i) {
            key += _keyword[i];
        }
        if (_keyword[i] == '|') ++i;
        Node a2(key , offset);
        keywordList.AddNode(a2);
    }
    strcpy(tmp.keyword , _keyword);
    w.seekp(offset);
    w.write(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    w.close();
};

void modifyPrice(double _price){
    int offset = users.top().select;
    fstream w;
    w.open("bookFile.dat" , ios::in | ios::out |ios::binary);
    w.seekg(offset);
    BOOK tmp;
    w.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    tmp.price = _price;
    w.seekp(offset);
    w.write(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    w.close();
};

void import(int _quantity , double totalPrice){
    int offset = users.top().select;
    fstream w;
    w.open("bookFile.dat" , ios::in | ios::out |ios::binary);
    if (!w) throw "open fail";
    w.seekg(offset);
    BOOK tmp;
    w.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    tmp.quantity += _quantity;
    w.seekp(offset);
    w.write(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    w.close();//
    w.open("financeFile.dat" , ios::in | ios::out | ios::binary);
    Finance tmp2;
    w.seekg(0);
    w.read(reinterpret_cast<char*>(&tmp2) , sizeof(Finance));
    tmp2.expense += totalPrice;
    w.seekp(0);
    w.write(reinterpret_cast<char*>(&tmp2) , sizeof(Finance));
    w.close();//
    w.open("transcationFile.dat" , ios::in | ios::out | ios::binary);
    Transcation tmp3;
    strcpy(tmp3.ISBN , tmp.ISBN);
    tmp3.cost -= totalPrice;
    tmp3.quantity = _quantity;
    w.seekp(0 , ios::end);
    w.write(reinterpret_cast<char*>(&tmp3) , sizeof(Transcation));
    w.close();
};

void buy(const char _isbn[] , int _quantity){
    if (!isbnList.existNode(_isbn)) throw "nofind";
    int offset = isbnList.FindNode(_isbn);
    fstream w;
    w.open("bookFile.dat" , ios::in | ios::out |ios::binary);
    if (!w) throw "open fail";
    w.seekg(offset);
    BOOK tmp;
    w.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    if (tmp.quantity < _quantity) throw "error";
    tmp.quantity -= _quantity;
    w.seekp(offset);
    w.write(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    w.close();//
    printf("%.2lf\n" , _quantity * tmp.price);
    w.open("financeFile.dat" , ios::in | ios::out | ios::binary);
    Finance tmp2;
    w.seekg(0);
    w.read(reinterpret_cast<char*>(&tmp2) , sizeof(Finance));
    tmp2.benefit += _quantity * tmp.price;
    w.seekp(0);
    w.write(reinterpret_cast<char*>(&tmp2) , sizeof(Finance));
    w.close();//
    w.open("transcationFile.dat" , ios::in | ios::out | ios::binary);
    Transcation tmp3;
    strcpy(tmp3.ISBN , tmp.ISBN);
    tmp3.cost += _quantity * tmp.price;
    w.seekp(0 , ios::end);
    w.write(reinterpret_cast<char*>(&tmp3) , sizeof(Transcation));
    w.close();
}

bool BOOK::operator<(const BOOK &o) {
    if (strcmp(ISBN , o.ISBN) < 0) return true;
    else return false;
};

void showISBN(string _ISBN){
    if (!isbnList.existNode(_ISBN)) {
        std::cout<<"\n";
        return;
    }
    vector<Node> t;
    t = isbnList.showNode(_ISBN , t);
    fstream r;
    r.open("bookFile.dat" , ios::in | ios::out |ios::binary);
    if (!r) throw "open fail";
    vector <BOOK> b;
    for (auto i : t){
        BOOK tmp;
        r.seekp(i.showOffset());
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
        b.push_back(tmp);
    }
    sort(b.begin() , b.end());
    for (auto it : b) it.show();
    r.close();
    /*int offset = isbnList.FindNode(_ISBN);
    fstream r;
    r.open("bookFile.dat" , ios::in | ios::out |ios::binary);
    if (!r) throw "open fail";
    BOOK tmp;
    r.seekp(offset);
    r.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
    tmp.show();*/
};

void showName(const string& _name){
    if (!nameList.existNode(_name)) {
        std::cout<<"\n";
        return;
    }
    vector<Node> t;
    t = nameList.showNode(_name , t);
    fstream r;
    r.open("bookFile.dat" , ios::in | ios::out |ios::binary);
    if (!r) throw "open fail";
    vector <BOOK> b;
    for (auto i : t){
        BOOK tmp;
        r.seekp(i.showOffset());
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
        b.push_back(tmp);
    }
    sort(b.begin() , b.end());
    for (auto it : b) it.show();
    r.close();
};

void showAuthor(const string& _author){
    if (!authorList.existNode(_author)) {
        std::cout<<"\n";
        return;
    }
    vector<Node> t;
    t = authorList.showNode(_author , t);
    fstream r;
    r.open("bookFile.dat" , ios::in | ios::out |ios::binary);
    if (!r) throw "open fail";
    vector <BOOK> b;
    for (auto i : t){
        BOOK tmp;
        r.seekp(i.showOffset());
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
        b.push_back(tmp);
    }
    sort(b.begin() , b.end());
    for (auto it : b) it.show();
    r.close();
};

void showKeyword(const string& _keyword){
    if (!keywordList.existNode(_keyword)) {
        std::cout<<"\n";
        return;
    }
    vector<Node> t;
    t = keywordList.showNode(_keyword, t);
    fstream r;
    r.open("bookFile.dat" , ios::in | ios::out |ios::binary);
    if (!r) throw "open fail";
    vector <BOOK> b;
    for (auto i : t){
        BOOK tmp;
        r.seekp(i.showOffset());
        r.read(reinterpret_cast<char*>(&tmp) , sizeof(BOOK));
        b.push_back(tmp);
    }
    sort(b.begin() , b.end());
    for (auto it : b) it.show();
    r.close();
};
