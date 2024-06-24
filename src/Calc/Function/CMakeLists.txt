add_subdirectory(Composite)
add_subdirectory(Impls)

add_library(Function SHARED FunctionBase.cpp)

target_link_libraries(Function Impls)
target_link_libraries(Function Composite)