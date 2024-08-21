add_library(Function SHARED FunctionBase.cpp
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

target_link_libraries(Function Calc)