#include <iostream>

#include "Log.h"
//#include "Testing.h"

using namespace std;


int main(int ArgsCount, char** Args)
{
    cout << " Welcome to Jason " << '\n'
         << "------------------" << '\n'
         << "   Version 0.1.0  " << '\n' << '\n';

    Logger l("run.log");
    l << Info << "Starting up Jason" << EndLog;

    //l << Debug << "Indexing previous variables" << EndLog;

    l << Info << "Exiting Jason, Exit Code 0" << EndLog;
    return 0;
}
