cmake_minimum_required(VERSION 3.27)
project(Jason)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

#[[add_executable(Jason src/Entry.cpp

        src/DateTime.cpp
        src/Log.cpp

        src/IO/IoThread.cpp
        src/IO/VarStorage.cpp

        src/Calc/StdCalc.cpp
        src/Calc/VariableType.cpp

        src/Calc/Function/FunctionBase.cpp
        src/Calc/Function/Composite/Polynomial.cpp
        src/Calc/Function/Impls/GeneralFunctions.cpp
        src/Calc/Function/Impls/VectorFunction.cpp
        src/Calc/Function/Impls/Core/AbsoluteValue.cpp
        src/Calc/Function/Impls/Core/Constant.cpp
        src/Calc/Function/Impls/Core/Exponent.cpp
        src/Calc/Function/Impls/Core/FnMonomial.cpp
        src/Calc/Function/Impls/Core/Logarithm.cpp
        src/Calc/Function/Impls/Core/Monomial.cpp
        src/Calc/Function/Impls/Core/PFnMonomial.cpp
        src/Calc/Function/Impls/Core/Trig.cpp

        src/Calc/Numerics/Scalar.cpp
        src/Calc/Numerics/MathVector.cpp
        src/Calc/Numerics/Matrix.cpp
)]]
