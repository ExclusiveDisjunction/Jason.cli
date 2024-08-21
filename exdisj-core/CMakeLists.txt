cmake_minimum_required(VERSION 3.28)
project(exdisj_core)

set(CMAKE_CXX_STANDARD 20)

add_library(exdisj_core SHARED DateTime.cpp
Log.cpp)
