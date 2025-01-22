#pragma once

#include "../../Calc/StdCalc.h"

class MATH_LIB FunctionBase;

class MATH_LIB General
{
public:
    General() = delete;
    General(const General& Obj) = delete;
    General(const General&& Obj) = delete;
    ~General() = default;

    General& operator=(const General& Obj) = delete;
    General& operator=(const General&& Obj) = delete;

    static FunctionBase* Linear(unsigned int Dim, unsigned int Var, double M = 1.0, double K = 0.0);
    static FunctionBase* Quadratic(unsigned int Dim, unsigned int Var, double A = 1.0, double B = 0.0, double C = 0.0);
    static FunctionBase* Cubic(unsigned int Dim, unsigned int Var, double A = 1.0, double B = 0.0, double C = 0.0, double D = 0.0);
    static FunctionBase* SquareRoot(unsigned int Dim, unsigned int Var, double A = 1.0, double _N = 1.0);
    static FunctionBase* Sine(unsigned int Dim, unsigned int Var, double A = 1.0, double w = 1.0, double phi = 0.0, double C = 0.0);
    static FunctionBase* Cosine(unsigned int Dim, unsigned int Var, double A = 1.0, double w = 1.0, double phi = 0.0, double C = 0.0);
};