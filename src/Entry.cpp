#include <iostream>

#include "Log.h"
#include "IO/PackageHeader.h"
#include "Commands/CommandParser.h"

int main(int ArgsCount, char** Args)
{
    std::cout << " Welcome to Jason " << '\n'
              << "------------------" << '\n'
              << "   Version " << JASON_CURRENT_VERSION << "  " << '\n' << '\n';

    Logger l("run.log");
    l << Info << "Starting up Jason" << EndLog;



    CommandParser cm;

    try
    {
        while (true)
        {
            std::cout << "Enter a Command Expression to be parsed: ";
            std::cin >> cm;
            std::cout << "Command was: " << cm << std::endl;
        }
    }
    catch (std::logic_error& e)
    {
        std::cerr << "Caught: " << e.what() << std::endl;
    }

    l << Info << "Exiting Jason, Exit Code 0" << EndLog;
    return 0;
}
