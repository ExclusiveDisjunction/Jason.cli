cmake_minimum_required(VERSION 3.28)
project(Jason)

set(CMAKE_CXX_STANDARD 20)

add_executable(Jason Entry.cpp
        DateTime.cpp
        Log.cpp)
