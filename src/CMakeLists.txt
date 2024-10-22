add_subdirectory(Calc)
add_subdirectory(Function)
add_subdirectory(Expressions)
add_subdirectory(IO)
add_subdirectory(Commands)

add_library(src SHARED DateTime.cpp
        Log.cpp
        Testing.cpp
        IOTester.h
        IOTester.cpp)

# PackageEntry.cpp is handled by topmost CMakeFile.

target_link_libraries(src Calc)
target_link_libraries(src Function)
target_link_libraries(src IO)
target_link_libraries(src Expressions)
target_link_libraries(src Commands)
