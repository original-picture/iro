#include <iostream>

#define IRO_IMPL
#include "iro.h"

using namespace iro;

persist print_stuff() {
    return std::cout << bright_blue << "blue in the function\n";
}

int main() {
    std::cout << "normal\n";
    auto p0 = print_stuff();
    std::cout << "blue\n";

    p0 << bold;
    {
        auto p1 = std::cout << bright_red << "red\n" << "red\n";
        persist p2;
        {
            p2 = std::cout << bright_green << "green\n";
        }
        std::cout << "green\n";
    }
    std::cout << "blue again\n";
    return 0;
}
