#include <iostream>

#define IRO_IMPL
#include "iro.h"

using namespace iro;

int main() {
    std::cout << "this text is normal\n";
    std::cout << iro::bright_green << "this text is green\n";
    std::cout << "this text is normal\n";

    {
        // I'm going to write out the type here for the sake of clarity,
        // but I recommend you use auto normally
        terminal_state_guard tsg0 = std::cout << bright_green << bold; // make text printed from cout bright green and bold
        std::cout << "  this text is green and bold\n";

        {
            auto tsg1 = std::cout << normal_weight << bright_red;
            std::cout << "      this text is red\n";

            {
                terminal_state_guard tsg3 = tsg0;
                std::cout << "          this text is green and bold\n";

                {
                    terminal_state_guard tsg2(std::cout);
                    tsg2 = tsg1;
                    std::cout << "              this text is red\n";
                }

                std::cout << "          this text is green and bold " << imbue(bright_blue, "(now it's blue)") << " and no it's green and bold again\n";
            }
            std::cout << "      this text is red\n";
        }

        std::cout << "  this text is green and bold\n";
    }
    std::cout << "this text is normal\n";
}
