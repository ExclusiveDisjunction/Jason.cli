cmake_minimum_required(VERSION 3.27)
project(Jason)

set(CMAKE_CXX_STANDARD 20)

add_executable(Jason src/Entry.cpp

        src/DateTime.cpp
        src/Log.cpp

        src/IO/IoThread.cpp
        src/IO/VarStorage.cpp

        src/Calc/StdCalc.cpp
        src/Calc/VariableType.cpp

        src/Calc/Function/FunctionBase.cpp

        src/Calc/Numerics/Scalar.cpp
        src/Calc/Numerics/MathVector.cpp
        src/Calc/Numerics/Matrix.cpp
)
