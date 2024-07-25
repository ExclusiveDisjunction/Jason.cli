//
// Created by exdisj on 7/25/24.
//

#include "Testing.h"

#include "Calc/Numerics.h"

#include <iostream>
using namespace std;

void testCalc()
{
    cout << "Test One: Scalars" << endl << endl;
    {
        Scalar a(3.4), b(2.1), c(-1.7);
        cout << "Sterilized a: ";
        a.Sterilize(cout);
        cout << endl;

        std::stringstream tempB;
        b.Sterilize(tempB);

        auto* bFrom = Scalar::FromSterilize(tempB.str());
        cout << "b -> sterilized -> Scalar == b? (Expect yes) " << (b == *bFrom ? "yes" : "no") << endl;
        delete bFrom;

        Scalar d = a + b;
        cout << "a + b == 5.5? (expect yes) " << (d == 3.4 + 2.1 ? "yes" : "no") << ", d = " << d << endl;
        d = a - c;
        cout << "a - c == 5.1? (expect yes) " << (d == 3.4 - -1.7 ? "yes" : "no") << ", d = " << d << endl;
        d = b * c;
        cout << "b * c == -3.57? (expect yes) " << (d == 2.1 * -1.7 ? "yes" : "no") << ", d = " << d << endl;
        d = a / c;
        cout << "a / c == -2? (expect yes) " << (d == 3.4/-1.7 ? "yes" : "no") << ", d = " << d << endl;

        cout << "Testing print: (expect 3.4, 2.1, -1.7, -2) " << a << ", " << b << ", " << c << ", " << d << endl;
    }

    cout << endl << "Test Two: Vectors" << endl << endl;
    {
        MathVector a(3.2, 1.6), b(1.6, -1.9), c(1.6, 3.3, 4.5);

        MathVector d = a + b;
        auto result = std::make_pair(3.2+1.6, 1.6 + -1.9);
        cout << "a + b == (" << result.first << ", " << result.second << ") ? " << (d == MathVector(result.first, result.second) ? "yes" : "no") << ", d = " << d << endl;

        d = a - b;
        result = std::make_pair(3.2-1.6, 1.6 - -1.9);
        cout << "a - b == (" << result.first << ", " << result.second << ") ? " << (d == MathVector(result.first, result.second) ? "yes" : "no") << ", d = " << d << endl;

        d = a * 3.1;
        result = std::make_pair(3.2*3.1, 1.6 *3.1);
        cout << "a * 3.1 == (" << result.first << ", " << result.second << ") ? " << (d == MathVector(result.first, result.second) ? "yes" : "no") << ", d = " << d << endl;

        d = a / 1.6;
        result = std::make_pair(3.2/1.6, 1.6 / 1.6);
        cout << "a / 1.6 == (" << result.first << ", " << result.second << ") ? " << (d == MathVector(result.first, result.second) ? "yes" : "no") << ", d = " << d << endl;

        auto mag = c.Magnitude();
        auto expec = sqrt( pow(c[0], 2) + pow(c[1], 2) + pow(c[2], 2) );
        cout << "|c| == " << expec << "? " << (mag == expec ? "yes" : "no") << ", mag = " << mag << endl;

    }

    cout << endl << "Test Three: Matrix" << endl << endl;
    {
        Matrix a(2, 2, 3.4, 1.6, 2.1, 0.9),
                b = Matrix::Identity(2);

        cout << "a = \n" << a << "\nb = \n" << b << endl;

        Matrix c = a+b;
        cout << "a + b = \n" << c << endl;
        cout << "a - b = \n" << a - b << endl;
        cout << "a * b = \n" << a * b << endl;
        cout << "a / 1.3 = \n" << a / 1.3 << endl;

        Matrix d(3, 3, -1,    -4.23,  1.6,
                 2.34, -1.93, -4,
                 2,     0.55,  3);
        cout << endl << "d = \n" << d << endl;
        cout << endl << "d = " << PrintMatrixOneLine(d) << endl;
    }
}

