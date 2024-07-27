add_library(Operators SHARED Expression.cpp
            Operator.cpp
            SubExpression.cpp
)

target_link_libraries(Operators Calc)
target_link_libraries(Operators IO)