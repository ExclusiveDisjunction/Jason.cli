#include <iostream>
#include <chrono>
#include <unistd.h>
#include <pwd.h>


#include "Log.h"
#include "Calc/Numerics.h"
//#include "Testing.h"
#include "Commands/CommandParser.h"

using namespace std;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::microseconds;
using std::chrono::milliseconds;

//#include "IO/Session.h"
#include "IO/Package.h"

#include <cstring>

int main(int ArgsCount, char** Args)
{
    cout << " Welcome to Jason " << '\n'
         << "------------------" << '\n'
         << "   Version 0.1.0  " << '\n' << '\n';

    Logger l("run.log");
    l << Info << "Starting up Jason" << EndLog;

    /*
    auto* sess = new Session();

    if (!sess->Initiate())
    {
        std::cerr << "Initiation of session failed!" << std::endl;
        l << Info << "Exiting Jason (Reason: Failed to Initiate), Exit Code 1" << EndLog;
        return 1;
    }
    else
    {
        std::cout << "Session created, and all packages loaded properly." << std::endl;
        l << Info << "Jason Session & Packages loaded." << EndLog;
    }

    delete sess;
    */

    /*
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;

    std::filesystem::path landing = std::filesystem::path(homedir) / ".jason";
    std::filesystem::path location = landing / "usr";
    if (!std::filesystem::exists(location))
        std::filesystem::create_directories(location);

    Package* New = Package::OpenFromDirectory(location, 0);
    if (!New)
    {
        New = Package::NewPackage("usr", landing, 0);
        if (!New)
        {
            std::cerr << "Could not load Jason project." << std::endl;
            return 1;
        }
    }

    std::cout << "Modes:" << std::endl <<
                 "i: Insert Items Manually" << std::endl <<
                 "t: Group Insert + Time" << std::endl <<
                 "s: Load Speed Test" << std::endl <<
                 "d: Display current elements" << std::endl <<
                 "f: Full test (clear and update for different times)" << std::endl <<
                 "r: Remove all current objects" << std::endl <<
                 "q: Quit" << std::endl;
    std::cout << std::endl;
    while (true)
    {
        std::cout << "Enter Mode: ";
        char mode;
        std::cin >> mode;
        if (mode == 'q')
            break;

        switch (mode)
        {
            case 'i':
            {
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

                break;
            }
            case 't':
            {
                auto start = std::chrono::system_clock::now();
                std::cout << "Enter test base name: ";
                std::string testname;
                std::cin >> testname;

                std::vector<VariableType*> tests = {
                        new Scalar(4),
                        new MathVector(3, 0),
                        new Matrix(3, 3, 1, 2, 3, 4, 5, 6, 7, 8, 9)
                };

                for (unsigned i = 0; i < 100; i++)
                {
                    unsigned j = 0;
                    for (auto& val : tests)
                    {
                        std::stringstream thisName;
                        thisName << testname << i << j;
                        if (!New->AddEntry(thisName.str(), Variable, val->Clone()))
                            std::cout << "Failed at " << i;
                        j++;
                    }
                }

                auto duration = std::chrono::system_clock::now() - start;
                std::cout << "\tInsert Time Duration is (ms)" << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "(mu-s:" << std::chrono::duration_cast<std::chrono::microseconds>(duration).count() << ')' << std::endl;
                break;
            }
            case 's':
            {
                auto start = std::chrono::system_clock::now();
                (void)New->LoadAllEntries();
                auto duration = std::chrono::system_clock::now() - start;
                std::cout << "\tLoad Time Duration is (ms) " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " (mu-s:" << std::chrono::duration_cast<std::chrono::microseconds>(duration).count() << ')' << std::endl;
                break;
            }
            case 'd':
            {
                (void)New->LoadAllEntries();
                New->DisplayContents(std::cout);
                break;
            }
            case 'r':
            {
                New->RemoveAllEntries();
                break;
            }
            case 'f':
            {
                //This will clear the project, add n elements, record the time to add them, unload the project, and then load it again.
                std::vector<int> test_runs = {10, 100, 1'000, 10'000};
                std::vector<VariableType*> test_data = {
                        new Scalar(4),
                        new MathVector(3, 0),
                        new Matrix(3, 3, 1, 2, 3, 4, 5, 6, 7, 8, 9)
                };

                for (int& n : test_runs)
                {
                    New->RemoveAllEntries();
                    auto start = std::chrono::system_clock::now();

                    for (unsigned i = 0; i < n; i++)
                    {
                        unsigned j = 0;
                        for (auto& val : test_data)
                        {
                            std::stringstream thisName;
                            thisName << n << '\'' << i << '\'' << j;
                            if (!New->AddEntry(thisName.str(), Variable, val->Clone()))
                                std::cout << "Failed at " << i;
                            j++;
                        }
                    }

                    auto time_for_adding = std::chrono::system_clock::now();
                    New->UnloadAllEntries();
                    auto time_for_unload = std::chrono::system_clock::now();
                    bool loadingResult = New->LoadAllEntries();
                    auto time_for_load = std::chrono::system_clock::now();

                    std::cout << "N = " << n << std::endl;
                    std::cout << "Results are:" << std::endl;

                    auto total = time_for_load - start;
                    auto adding = time_for_adding - start;
                    auto unloading = time_for_unload - time_for_adding;
                    auto loading = time_for_load - time_for_unload;

                    std::cout <<
                    setw(9) << "Quantity"  << " | " << setw(7)                    << "Seconds"                         << " | " << setw(12)                     << "Milliseconds"                         << " | " << setw(12)                     << "Microseconds"                         << std::endl <<
                    setw(9) << "--------"  << " | " << setw(7)                    << "-------"                         << " | " << setw(12)                     << "------------"                         << " | " << setw(12)                     << "------------"                         << std::endl <<
                    setw(9) << "Total"     << " | " << setw(7) << setprecision(7) << duration_cast<seconds>(total)     << " | " << setw(12) << setprecision(12) << duration_cast<milliseconds>(total)     << " | " << setw(12) << setprecision(12) << duration_cast<microseconds>(total)     << std::endl <<
                    setw(9) << "Adding"    << " | " << setw(7) << setprecision(7) << duration_cast<seconds>(adding)    << " | " << setw(12) << setprecision(12) << duration_cast<milliseconds>(adding)    << " | " << setw(12) << setprecision(12) << duration_cast<microseconds>(adding)    << std::endl <<
                    setw(9) << "Unloading" << " | " << setw(7) << setprecision(7) << duration_cast<seconds>(unloading) << " | " << setw(12) << setprecision(12) << duration_cast<milliseconds>(unloading) << " | " << setw(12) << setprecision(12) << duration_cast<microseconds>(unloading) << std::endl <<
                    setw(9) << "Loading"   << " | " << setw(7) << setprecision(7) << duration_cast<seconds>(loading)   << " | " << setw(12) << setprecision(12) << duration_cast<milliseconds>(loading)   << " | " << setw(12) << setprecision(12) << duration_cast<microseconds>(loading)   << std::endl;
                    std::cout << std::endl << std::endl;
                }
                break;
            }
            default:
                std::cout << "Unknown option " << mode << std::endl;
                break;
        }
    }
    (void)New->Save();
     */

    CommandSingleValue sv;
    CommandMultiValue mv;

    std::cout << "Enter a a value for CommandSingleValue: ";
    std::cin >> sv;
    std::cout << "Value put: " << sv << std::endl;

    try
    {
        std::cout << "Enter a value for CommandMultiValue: ";
        std::cin >> mv;
        std::cout << "Value put: " << mv << std::endl;
    }
    catch (std::logic_error& e)
    {
        std::cerr << "Caught: " << e.what() << std::endl;
    }

    l << Info << "Exiting Jason, Exit Code 0" << EndLog;
    return 0;
}
