
#include <iostream>

#define IRO_IMPL
#include "iro.h"

int main() {
    std::cout << "this text is normal\n";
    // std::cout << iro::bright_green constructs an iro::terminal_state_guard that "remembers" that the terminal is currently green (and that will reset this state when it gets destroyed)
    std::cout << iro::bright_green << "this text is green\n"; // we can then print some text that the specified effect (bright_green) will apply to
    std::cout << "this text is normal\n";                // ^ and then the terminal_state_guard will get destroyed here, resetting the terminal to normal

    // that's the simplest way in which you can use the library
    // but you can do a lot more if you store the terminal_state_guard in a variable
    {
        // here you can see we store the terminal_state_guard we're given, instead of letting it get destroyed at the end of the expression
        iro::terminal_state_guard tsg0 = std::cout << iro::bright_green << iro::bold; // make text printed from cout bright green and bold
        std::cout << "  this text is green and bold\n"; // we're still holding onto tsg0, so the terminal has not been reset, and our text will still be green
        // (I've indented the text to emphasize how C++ scope affects the output)
        {
            // internally, iro effects are stored in a stack (with one stack per effect type) which is assigned to a particular ostream, as determined by the ostream's address
            // so when we do std::cout << normal_weight << bright_red below,
            // bright_red will get pushed to the foreground color stack of cout, and normal_weight will get pushed to its font weight stack
            // the effects that are on the top of their respective stacks are the effects that you will
            // destroying the terminal_state_guard associated with a given set of effects will remove the effects from the stack
            // I say "remove" and not "pop" because it's possible to remove effects that aren't at the top of their stacks
            // so I guess the stacks referred to here are really "pseudostacks" :P

            auto tsg1 = std::cout << iro::normal_weight << iro::bright_red;
            std::cout << "      this text is red\n";

            {
                iro::terminal_state_guard tsg3 = tsg0;                       // we can copy construct terminal state guards too.
                std::cout << "          this text is green and bold\n"; // This will simply push a new state guard with the same effect_set as the old one

                {
                    iro::terminal_state_guard tsg2(std::cout);
                    tsg2 = tsg1; // copy assignment works too, and does what you would expect
                    std::cout << "              this text is red\n";

                    // move construction and assignment work too (and behave how you would expect them to), but I haven't found many practical uses for them
                }

                // this is a different API!
                std::cout << "          this text is green and bold " << iro::imbue(iro::bright_blue, "(now it's blue)") << " and no it's green and bold again\n";
                // imbue lets you construct an object called an effect_string that contains embedded effects that don't affect the long-term effect state of the terminal
                // they're useful if you don't care about lifetime and just want a more traditional color printing API
                // you can concatenate effect_strings too, but that behavior isn't shown here
            } // tsg3 just died, so we're back to printing red text
            std::cout << "      this text is red\n";
        } // tsg1 just died, so we're back to printing green text

        std::cout << "  this text is green and bold\n";
    } // and tsg0 just died, so we're back to normal
    std::cout << "this text is normal\n";
    // and that's it! Look ma, no resets!
}
