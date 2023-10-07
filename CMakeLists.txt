cmake_minimum_required(VERSION 3.21)
project(iro2)

set(CMAKE_CXX_STANDARD 14)

add_executable(iro2 main.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    add_compile_options(-Werror=return-type)
endif()