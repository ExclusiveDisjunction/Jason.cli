add_subdirectory(Core)

add_library(Impls SHARED GeneralFunctions.cpp
        VectorFunction.cpp)

target_link_libraries(Impls Core)
