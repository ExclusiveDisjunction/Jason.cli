#include <iostream>
#include <chrono>

#include "Log.h"
//#include "Testing.h"

using namespace std;

#include "IO/Session.h"

int main(int ArgsCount, char** Args)
{
    cout << " Welcome to Jason " << '\n'
         << "------------------" << '\n'
         << "   Version 0.1.0  " << '\n' << '\n';

    Logger l("run.log");
    l << Info << "Starting up Jason" << EndLog;

    auto* sess = new Session();
    if (!sess->CheckForUnclosedPackages())
    {
        std::cout << "Goodbye!" << std::endl;
        l << Info << "Exiting Jason (Reason: Preserve Open Packages), Exit Code 2" << EndLog;
        return 2;
    }


    if (!sess->Initiate())
    {
        std::cerr << "Initiation of session failed!" << std::endl;
        l << Info << "Exiting Jason (Reason: Failed to Initiate), Exit Code 1" << EndLog;
        return 1;
    }
    else
    {
        std::cout << "Session created properly." << std::endl;
        l << Info << "Jason Session & Packages loaded." << EndLog;
    }

    delete sess;
    l << Info << "Exiting Jason, Exit Code 0" << EndLog;
    return 0;
}
