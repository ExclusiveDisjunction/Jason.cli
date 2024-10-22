#include <iostream>

#include "IOTester.h"

int main(int, char**)
{
    std::cout << " Welcome to Jason " << '\n'
              << "------------------" << '\n'
              << "   Version " << JASON_CURRENT_VERSION << "  " << '\n' << '\n';

    Logger l("run.log");
    l << Info << "Starting up Jason" << EndLog;

    IOTester();

    l << Info << "Exiting Jason, Exit Code 0" << EndLog;
    return 0;
}
