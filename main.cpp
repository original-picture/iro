#include <iostream>

#define IRO_IMPL
#include "iro.h"

using namespace iro;

persist print_stuff() {
    return std::cout << bright_blue << "ファンクションの中の青\n";
}

int main() {
/*
    std::cout << "普通\n";
    auto p0 = print_stuff();
    std::cout << "青\n";

    p0 << bold;
    {
        auto p1 = std::cout << bright_red << "赤\n";
        persist p2(std::cout);
        {
            p2 = std::cout << bright_green << effect_string(red, "now red ") << "緑\n";
        }
        std::cout << "緑\n";
    }
    std::cout << "また青\n";

    std::cerr << "still blue\n";*/
  /*  auto p0 = std::cout << bright_red << "red\n";
    auto p1 = std::cout << bright_green << "green\n";

    {
        auto p2 = std::cout << bright_blue << "blue\n";

        p1 = std::move(p2);
    }

    std::cout <<  "Should be blue\n";

    return 0;*/

    /*
    {
        auto p = std::cout << bright_green;
        auto p0 = std::cout << bold;
        std::cout << "green and bold\n";

        {
            auto p1 = std::move(p0);
            p << bright_red;
            std::cout << "red and bold\n";
        }
        std::cout << "just red\n";
    }

    std::cout << "normal\n";*/


    std::cout << "normal\n";
    {
        auto p0 = std::cout << bright_green;
        std::cout << "green\n";

        {
            auto p1 = std::cout << bold;
            std::cout << "green and bold\n";

            p0 << bright_blue;
            std::cout << "blue and bold\n";
        }
        std::cout << "blue\n";

    }
    std::cout << "normal\n";


    /*
    auto str = effect_string(normal_weight, "green\n");
    str += effect_string(red, "red and bold\n");

    auto e = bright_green|bold;

    auto p0 = std::cout << e;

    std::cout << "green and bold\n";

    std::cout << str.unsafe_string(std::cout);

    std::cout << "green and bold\n";*/

    /*
    {
        auto p0 = std::cout << bright_green;
        std::cout << "green\n";

        persist p1(std::cout);
        std::cout << "green\n";
        {
            auto p2 = std::cout << bright_red;
            std::cout << "red\n";

            p1 = std::move(p2);
            std::cout << "what color?\n";
        }

        std::cout << "red\n";
    }

    std::cout << "normal\n";*/

    /*std::cout << effect_string(bright_green|bold, "hi", 32.1f, '\n');
    std::cout << "normal\n";*/
}
