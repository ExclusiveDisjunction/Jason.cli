add_subdirectory(Calc)
add_subdirectory(Function)
add_subdirectory(Operators)
add_subdirectory(IO)

add_library(src DateTime.cpp
        Log.cpp
        Testing.cpp)

# Entry.cpp is handled by topmost CMakeFile.

target_link_libraries(src Calc)
target_link_libraries(src Function)
#target_link_libraries(src IO)
target_link_libraries(src Operators)
