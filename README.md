# Iro
Iro is a header only C++14 library for printing text with color and other effects achievable with [ANSI escape sequences](https://en.wikipedia.org/wiki/ANSI_escape_code)
Its defining feature is that it used [RAII](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization) to automatically reset terminal effects, 
so unlike other fancy printing libraries, you don't have to remember to call some `reset()` function to 

## Example
copy-pasted from example.cpp:
```cpp
#include <iostream>

#define IRO_IMPL
#include "iro.h"

using namespace iro;

int main() {
    std::cout << "normal\n";
    {
        auto p0 = std::cout << bright_green << bold;
        std::cout << "green and bold\n";

        {
            auto p1 = std::cout << normal_weight << bright_red;
            std::cout << "red\n";

            {
                terminal_state_guard p3 = p0;
                std::cout << "green and bold\n";

                {
                    terminal_state_guard p2(std::cout);
                    p2 = p1;
                    std::cout << "red\n";
                }

                std::cout << "green and bold " << imbue(bright_blue, "blue") << " green and bold\n";
            }
            std::cout << "red\n";
        }

        std::cout << "green and bold\n";
    }
    std::cout << "normal\n";
}
```

## Build
It's a header-only library, so just do `#define IRO_IMPL` before `#include "iro.h"` in one .cpp file, and then include it normally everywhere else
```cpp
#define IRO_IMPL // only do this in ONE .cpp file! And it has to be BEFORE #include "iro.h"
#include "iro.h"

int main() {
    // go crazy...
}
```

To clone the libary and build the example:
```shell
git clone https://github.com/original-picture/iro
cd iro
cmake -B build -S .
cd build
cmake --build .
```

## Features
* supported effect types
  * foreground_color (16 colors)
  * background_color (16 colors)
  * font_weight 
    * bold
    * faint
    * normal
  * underlinedness
  * blink
* 

## High-level documentation
I'm going to start by explaining a snippet of code from example.cpp
```cpp
std::cout << iro::bright_green << "this text is green\n";
std::cout << "this text is normal\n";
```
On the first line, we insert some object called `iro::bright_green` into the stream `std::cout`.
Without knowing anything bout iro, you would probably correctly guess that the following text would come out green   
But why is the text on the following line normal again? 
This is because iro defines an overload of the operator `<<` that takes a `std::ostream&` and an `iro::effect` (the type of `iro::bright_green`) and returns an `iro::terminal_state_guard`  
`This class, `terminal_state_guard` is the backbone of the library. `terminal_state_guard` 







Most of the library is built around a class called `terminal_state_guard`.
It's like a `std::lock_guard`, but instead of resetting a `std::mutex` to its unlocked state,
it resets the terminal's state to how it was at some earlier point in time.  
You construct a `terminal_state_guard` with a `std::ostream&`. Because of the implicit conversions involved, that will usually look something like this:
```cpp
terminal_state_guard tsg = std::cout;
```
But in that example, we haven't applied any effects to std::cout, so it isn't a very interesting example.
The typical use will look more like this:
```cpp
terminal_state_guard
```

## API reference
> [!WARNING]
> This library does use global state! If you use iro functionality from multiple threads, you'll need to synchronize your calls yourself (probably by locking a mutex before calling an iro functions)

## TODO
- [ ] better documentation
- [ ] 256 color support
- [ ] clean up the platform specific code
- [ ] hoisting up effects that are buried in the stack