#include <iostream>

#include "Log.h"

using namespace std;

int main(int ArgsCount, char** Args)
{
    cout << " Welcome to Calc-Cmd " << '\n'
         << "---------------------" << '\n'
         << "    Version 0.1.0    " << '\n' << '\n';

    Logger l("run.log");
    l << Info << "Abcde" << EndLog;
}