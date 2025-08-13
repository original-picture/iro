

#include <iostream>

#define IRO_IMPL
#include "iro.h"

int main() {
   std::cout << "this text is normal\n";
   std::cout << iro::bright_blue << "this text is blue\n"; // automatically resets here
   std::cout << "this text is normal\n";

    {
        // the blue effect will be automatically reset when tsg is destroyed
        iro::terminal_state_guard tsg = std::cout << iro::bright_blue << "  this text is blue\n";
        std::cout << "  this text is still blue\n";

        {
            iro::terminal_state_guard tsg1 = std::cout << iro::bright_red << iro::underline;
            std::cout << "    this text is red and underlined\n";

            {
                iro::terminal_state_guard tsg2 = std::cout << iro::bright_green;
                std::cout << "      this text is green and still underlined\n";
            } // tsg2 destroyed
            std::cout << "    this text is red and underlined again\n";
        } // tsg1 destroyed
        std::cout << "  this text is blue again\n";

    } // tsg destroyed
    std::cout << "this text is normal again\n";
}
