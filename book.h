//
// Created by lenovo on 2021/1/31.
//
#include "Blocklist.h"
#include <algorithm>

#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H

class BOOK{
private:
    char ISBN[20] = "" ;
    char name[60] = "" ;
    char author[60] = "" ;
    char keyword[60] = "" ;
    double price = 0;
    int quantity = 0;

public:
    BOOK() = default;
    BOOK(const string& _isbn , const string& _name , const string& _author , const string& _keyword , double _price , int _quantity);
    void show() const;
    bool operator<(const BOOK &o);

    friend void modifyISBN(const char _ISBN[]);
    friend void modifyName(const char _name[]);
    friend void modifyAuthor(const char _author[]);
    friend void modifyKeyword(const char _keyword[]);
    friend void modifyPrice(double _price);
    friend void import(int _quantity , double totalPrice);
    friend void buy(const char _isbn[] , int _quantity);
};

void select(const char isbn[]);

void modifyISBN(const char _ISBN[]);

void modifyName(const char _name[]);

void modifyAuthor(const char _author[]);

void modifyKeyword(const char _keyword[]);

void modifyPrice(double _price);

void import(int _quantity , double totalPrice);

void buy(const char _isbn[] , int _quantity);

void showISBN(string _ISBN);

void showName(const string& _name);

void showAuthor(const string& _author);

void showKeyword(const string& _keyword);

#endif //BOOKSTORE_BOOK_H
