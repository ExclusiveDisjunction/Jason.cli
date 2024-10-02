#include <iostream>
#include <chrono>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


#include "Log.h"
#include "Calc/Numerics.h"
//#include "Testing.h"

using namespace std;

//#include "IO/Session.h"
#include "IO/Package.h"

int main(int ArgsCount, char** Args)
{
    cout << " Welcome to Jason " << '\n'
         << "------------------" << '\n'
         << "   Version 0.1.0  " << '\n' << '\n';

    Logger l("run.log");
    l << Info << "Starting up Jason" << EndLog;

    /*
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
    */

    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    std::filesystem::path location = std::filesystem::path(homedir) / ".jason" / "usr";
    if (!std::filesystem::exists(location))
        std::filesystem::create_directories(location);

    Package* New = Package::OpenFromDirectory(location, 0);
    std::cout << "Package Contents: " << std::endl;
    New->DisplayContents(std::cout);
    std::cout << std::endl << std::endl;

    while (true)
    {
        std::cout << "Enter dimension count followed by numbers (-1 to exit). ";
        int dim;
        std::cin >> dim;
        if (dim < 0)
            break;

        MathVector v(dim, 0);
        for (int i = 0; i < dim; i++)
            std::cin >> v[i];
        std::cout << "Vector is " << v << std::endl;
        std::cout << "Enter the name: ";
        std::string name;
        std::cin >> name;

        auto result = New->AddEntry(name, PackageEntryType::Variable, v.MoveIntoPointer());
        if (!result.has_value())
            std::cout << "Failed" << std::endl;
        else
            std::cout << "Key is " << *result << std::endl;
    }

    (void)New->Save();

    l << Info << "Exiting Jason, Exit Code 0" << EndLog;
    return 0;
}
