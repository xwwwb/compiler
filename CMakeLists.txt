cmake_minimum_required(VERSION 3.24)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

set(Class 1)

IF(Class EQUAL 1)
    add_executable(compiler src/test/demo.cpp)
ELSEIF (Class EQUAL 0)
    add_executable(compiler src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/lexical.cpp src/lexical/main.cpp)
ENDIF()
