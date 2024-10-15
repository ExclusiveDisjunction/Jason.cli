add_library(Expressions SHARED Expression.cpp
            Operator.cpp
            SubExpression.cpp
)

target_link_libraries(Expressions Calc)
target_link_libraries(Expressions IO)