#include <iostream>

#define IRO_IMPL
#include "iro.h"

using namespace iro;

persist print_stuff() {
    return std::cout << blue << "blue in the function\n";
}

int main() {
    std::cout << "normal\n";
    auto p0 = print_stuff();
    std::cout << "blue\n";
    {
        auto p1 = std::cout << red << "red\n" << "red\n";
        persist p2;
        {
            p2 = std::cout << green << "green\n";
        }
        std::cout << "green\n";
    }
    std::cout << "blue again\n";
    return 0;
}
