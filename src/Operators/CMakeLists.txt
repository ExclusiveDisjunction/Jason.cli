add_library(Operators SHARED Expression.cpp
            Operator.cpp)

target_link_libraries(Operators Calc)