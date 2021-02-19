cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(bookstore
        main.cpp
        Blocklist.h
        Blocklist.cpp
        book.h
        book.cpp
        program.h
        program.cpp
        user.cpp
        user.h
        store.cpp
        store.h)