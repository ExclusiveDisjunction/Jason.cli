add_subdirectory(Calc)
add_subdirectory(Function)
add_subdirectory(Operators)
add_subdirectory(IO)

add_executable(Jason Entry.cpp
        DateTime.cpp
        Log.cpp
        Testing.h
        Testing.cpp)

target_link_libraries(Jason Calc)
target_link_libraries(Jason Function)
#target_link_libraries(Jason IO)
#target_link_libraries(Jason Operators)