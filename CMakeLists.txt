cmake_minimum_required(VERSION 3.27)
project(Jason)

set(CMAKE_CXX_STANDARD 20)

add_executable(Jason src/Entry.cpp
        src/DateTime.cpp
        src/Log.cpp
        src/IO/VarStorage.cpp
        src/IO/VarStorage.h
        src/Calc/VariableType.h)
