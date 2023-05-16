cmake_minimum_required(VERSION 3.24)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

set(Class 0) # 0 for LL1, 1 for recursive descent method 2 for lexical

IF (Class EQUAL 0)
    add_executable(compiler src/syntax/LL1/LL1.cpp src/syntax/LL1/LL1.h src/syntax/LL1/main.cpp)
ELSEIF (Class EQUAL 1)
    add_executable(compiler src/syntax/recursive_descent_method.cpp)
ELSEIF (Class EQUAL 2)
    add_executable(compiler src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/main.cpp)
ENDIF ()
