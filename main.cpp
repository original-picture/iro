#include <iostream>

#define IRO_IMPL
#include "iro.h"

using namespace iro;

persist print_stuff() {
    return std::cout << bright_blue << "ファンクションの中の青\n";
}

int main() {

    std::cout << "普通\n";
    auto p0 = print_stuff();
    std::cout << "青\n";

    p0 << bold;
    {
        auto p1 = std::cout << bright_red << "赤\n";
        persist p2;
        {
            p2 = std::cout << bright_green << effect_string(red, "now red ") << "緑\n";
        }
        std::cout << "緑\n";
    }
    std::cout << "また青\n";

    std::cerr << "still blue\n";

  /*  auto p0 = std::cout << bright_red << "red\n";
    auto p1 = std::cout << bright_green << "green\n";

    {
        auto p2 = std::cout << bright_blue << "blue\n";

        p1 = std::move(p2);
    }

    std::cout <<  "Should be blue\n";

    return 0;*/
}
