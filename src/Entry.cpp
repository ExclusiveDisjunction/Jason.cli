#include <iostream>

#include "Log.h"

using namespace std;

int main(int ArgsCount, char** Args)
{
    cout << " Welcome to Jason " << '\n'
         << "------------------" << '\n'
         << "   Version 0.1.0  " << '\n' << '\n';

    cout << "Hello there" << endl;

    Logger l("run.log");
    l << Info << "Starting up Jason" << EndLog;

    l << Debug << "Indexing previous variables" << EndLog;

    return 0;
}
