cmake_minimum_required(VERSION 3.27)
project(Jason)

set(CMAKE_CXX_STANDARD 20)

add_executable(Jason src/Entry.cpp
        src/DateTime.cpp
        src/Log.cpp
        src/IO/VarStorage.cpp
        src/IO/VarStorage.h
        src/Calc/VariableType.h
        src/Calc/Scalar.cpp
        src/Calc/Scalar.h
        src/Calc/StdCalc.cpp
        src/Calc/MathVector.cpp
        src/Calc/VariableType.cpp
        src/Calc/Matrix.cpp
        src/Calc/OperatorException.h
        src/IO/IoThread.cpp
        src/IO/IoThread.h
        src/Calc/Function/Function.h
        src/Calc/Function/Impls/CoreFunctions.h
        src/Calc/Function/Base/FunctionBase.h
        src/Calc/Function/Base/FunctionRelation.h
        src/Calc/Function/Base/FunctionRelation.cpp
        src/Calc/Function/Base/FunctionBase.cpp
        src/Calc/Complex.cpp
        src/Calc/Complex.h)
