cmake_minimum_required(VERSION 3.27)
project(Jason)

set(CMAKE_CXX_STANDARD 20)

add_executable(Jason src/Entry.cpp
        src/DateTime.cpp
        src/Log.cpp
        src/IO/VarStorage.cpp
        src/IO/VarStorage.h
        src/Calc/VariableType.h
        src/Calc/Numerics/Scalar.cpp
        src/Calc/Numerics/Scalar.h
        src/Calc/StdCalc.cpp
        src/Calc/Numerics/MathVector.cpp
        src/Calc/VariableType.cpp
        src/Calc/Numerics/Matrix.cpp
        src/Calc/OperatorException.h
        src/IO/IoThread.cpp
        src/IO/IoThread.h
        src/Calc/Function/Function.h
        src/Calc/Function/Impls/CoreFunctions.h
        src/Calc/Function/FunctionBase.h
        src/Calc/Function/FunctionBase.cpp
        src/Calc/Numerics/Complex.cpp
        src/Calc/Numerics/Complex.h
        src/Calc/Numerics.h)
