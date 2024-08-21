cmake_minimum_required(VERSION 3.28)
project(jason_functions)

set(CMAKE_CXX_STANDARD 20)

add_library(jason_functions SHARED FunctionBase.cpp
        FunctionIterator.cpp

        Impls/GeneralFunctions.cpp
        Impls/VectorFunction.cpp
        Impls/Bezier.cpp

        Impls/Core/AbsoluteValue.cpp
        Impls/Core/Constant.cpp
        Impls/Core/Exponent.cpp
        Impls/Core/Logarithm.cpp
        Impls/Core/FnMonomial.cpp
        Impls/Core/Monomial.cpp
        Impls/Core/PFnMonomial.cpp
        Impls/Core/Trig.cpp

        Composite/Polynomial.cpp
        Composite/RationalFunction.cpp)

