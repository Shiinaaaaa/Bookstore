//
// Created by lenovo on 2021/1/31.
//
//#include <string>

#include <string>
#include "user.h"
#include "book.h"
#include "store.h"

#ifndef BOOKSTORE_RUN_H
#define BOOKSTORE_RUN_H

void initial();

void run(const std::string& command);

void exit();

void quit();

void debug();

#endif //BOOKSTORE_RUN_H
