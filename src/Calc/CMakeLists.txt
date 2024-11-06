add_library(Calc SHARED StdCalc.cpp
    VariableType.cpp
    Numerics/Scalar.cpp
    Numerics/Matrix.cpp
    Numerics/MathVector.cpp
)

target_link_libraries(Calc Core)