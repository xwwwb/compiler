cmake_minimum_required(VERSION 3.24)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

set(Class 4) # 0 for LL1, 1 for recursive descent method 2 for lexical 3 for operator precedence parsing

IF (Class EQUAL 0)
    add_executable(compiler src/syntax/LL1/LL1.cpp src/syntax/LL1/LL1.h src/syntax/LL1/main.cpp)
ELSEIF (Class EQUAL 1)
    add_executable(compiler src/syntax/recursive_descent_method.cpp)
ELSEIF (Class EQUAL 2)
    add_executable(compiler src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/main.cpp)
ELSEIF (Class EQUAL 3)
    add_executable(compiler src/syntax/OOP/main.cpp src/syntax/OOP/OOP.h src/syntax/OOP/OOP.cpp)
ELSEIF (Class EQUAL 4)
    add_executable(compiler src/mid_code/main.cpp)

ELSE ()
    message(FATAL_ERROR "Class is not defined")
ENDIF ()
