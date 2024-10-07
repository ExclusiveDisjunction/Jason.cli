add_library(Commands SHARED CommandBase.cpp
        CommandParser.cpp
        CommandParser.h
        CommandValue.cpp
        CommandSpecifier.cpp)

target_link_libraries(Commands Calc)
target_link_libraries(Commands IO)