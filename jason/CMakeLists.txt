cmake_minimum_required(VERSION 3.27)
project(Jason)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

add_executable(Jason src/Entry.cpp
        src/Common.h
        src/Common.cpp)

target_link_libraries(Jason src)
