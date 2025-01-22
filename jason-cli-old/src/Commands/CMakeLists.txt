add_library(Commands SHARED CommandBase.cpp
        CommandParser.cpp
        CommandParser.h
        CommandValue.cpp
        CommandSpecifier.cpp
        CommandSpecifier.h
        CommandValue.h
        JasonCommands.h
        CommandsCode/EvaluateCommand.cpp)

target_link_libraries(Commands Calc)
target_link_libraries(Commands IO)
target_link_libraries(Commands Expressions)