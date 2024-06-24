add_subdirectory(Function)
add_subdirectory(Numerics)

add_library(Calc SHARED StdCalc.cpp
    VariableType.cpp)

target_link_libraries(Calc Function)
target_link_libraries(Calc Numerics)