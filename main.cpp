#include <iostream>
#include "program.h"

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
    return 0;
}
