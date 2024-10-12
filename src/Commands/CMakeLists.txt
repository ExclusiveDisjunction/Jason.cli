add_library(Commands SHARED CommandBase.cpp
        CommandParser.cpp
        CommandParser.h
        CommandValue.cpp
        CommandSpecifier.cpp
        CommandSingleValue.cpp
        CommandMultiValue.cpp
        CommandSpecifier.h
        CommandValue.h)

target_link_libraries(Commands Calc)
target_link_libraries(Commands IO)