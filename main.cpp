#include <iostream>
#include "program.h"

//改为二分查找
//userid 的 public
//是否判断已经登陆
//su 能否7
//logout
//delete
//if (o < tmp.nodes[0] && tmp.previous != -1) {
//show 只查了后一个
//mergr 问题

using namespace std;
int main() {
    string command;

    try {
        initial();
    }
    catch (...) {
        std::cout << "Invalid\n";
    }

    while(getline(cin , command)){
        try {
            run(command);
        }
        catch (...) {
            std::cout << "Invalid\n";
        }
    }

}
