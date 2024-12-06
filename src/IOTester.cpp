//
// Created by Hollan on 10/18/24.
//

#include "IOTester.h"

void IOTester() noexcept
{
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;

    std::filesystem::path landing = std::filesystem::path(homedir) / ".jason";
    std::filesystem::path location = landing / "usr";
    if (!std::filesystem::exists(landing))
        std::filesystem::create_directories(location);

    std::shared_ptr<Package> New;

    try 
    {
        New = Package::OpenFromDirectory(location, 0);
    }
    catch (std::logic_error& e)
    {
        try 
        {
            New = Package::NewPackage("usr", landing, 0);
        }
        catch (std::logic_error& e)
        {
            std::cerr << "Could not load Jason package because '" << e.what() << "'\n";
            return;
        }
    }

    std::cout << "Commands: " << std::endl <<
              "insert: Insert items via name and sterilized format" << std::endl <<
              "load: Load all objects & time" << std::endl <<
              "display: Display current elements" << std::endl <<
              "test: Full test (clear & update for different counts)" << std::endl <<
              "reset: Remove all current objects" << std::endl <<
              "lookup: Lookup an entry based on name or key" << std::endl <<
              "remove: Remove an entry based on name or key" << std::endl <<
              "help: Get help on a command, or list commands" << std::endl <<
              "quit: Save & Quit" << std::endl << std::endl;

    while (true)
    {
        std::cout << "> ";
        CommandParser parser;
        try
        {
            std::cin >> parser;
        }
        catch (std::logic_error& e)
        {
            std::cout << "Error while parsing command: " << e.what() << std::endl;
        }
        const std::string& mode = parser.TopCommand();

        if (mode == "quit")
            break;
        else if (mode == "insert")
        {
            if (parser.Values().size() < 2)
            {
                std::cout << "Invalid amount of arguments. At least two values are expected" << std::endl;
                continue;
            }

            std::string name = parser.Values()[0].Value;
            std::stringstream sterilized(parser.Values()[1].Value);
            double value;
            sterilized >> value;

            std::unique_ptr<VariableType> extracted = std::make_unique<Scalar>(value);

            auto result = New->AddEntry(name, PackageEntryType::Variable, std::move(extracted));
            if (!result.has_value())
                std::cerr << "Failed to insert into package" << std::endl;
            else
                std::cout << "Key is " << *result << std::endl;

            std::cerr.flush();
        }
        else if (mode == "load")
        {
            auto start = std::chrono::system_clock::now();
            (void)New->LoadAllEntries();
            auto duration = std::chrono::system_clock::now() - start;
            std::cout << "\tLoad Time Duration is (ms) " << duration_cast<milliseconds>(duration).count() << " (mu-s:" << duration_cast<microseconds>(duration).count() << ')' << std::endl;
        }
        else if (mode == "display")
        {
            (void)New->LoadAllEntries();
            New->DisplayContents(std::cout);
        }
        else if (mode == "test")
        {
            //This will clear the project, add cols elements, record the time to add them, unload the project, and then load it again.
            std::vector<int> test_runs = {10, 100, 1'000};
            std::vector<std::unique_ptr<VariableType>> test_data;
            test_data.emplace_back(new Scalar(4));
            test_data.emplace_back(new MathVector(MathVector::FromList(3, 0)));
            test_data.emplace_back(new Matrix(Matrix::FromList(3, 3, 1, 2, 3, 4, 5, 6, 7, 8, 9)));

            for (int& cols : test_runs)
            {
                New->RemoveAllEntries();
                auto start = std::chrono::system_clock::now();

                for (unsigned i = 0; i < cols; i++)
                {
                    unsigned j = 0;
                    for (auto& val : test_data)
                    {
                        std::stringstream thisName;
                        thisName << cols << '\'' << i << '\'' << j;
                        if (!New->AddEntry(thisName.str(), Variable, val->Clone()))
                            std::cout << "Failed at " << i;
                        j++;
                    }
                }

                (void)New->Save();
                std::cout << "STOPPED";
                char c;
                std::cin >> c;
                if (c == 'q')
                {
                    (void)New->Save();
                    std::cout << "Exiting";
                    return;
                }

                auto time_for_adding = std::chrono::system_clock::now();
                New->UnloadAllEntries();
                auto time_for_unload = std::chrono::system_clock::now();
                (void)New->LoadAllEntries();
                auto time_for_load = std::chrono::system_clock::now();

                std::cout << "N = " << cols << std::endl;
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

            New->RemoveAllEntries();
        }
        else if (mode == "reset")
            New->RemoveAllEntries();
        else if (mode == "lookup")
        {
            std::cout << "Sorry, this is not implemented yet." << std::endl;
        }
        else if (mode == "remove")
        {
            std::cout << "Sorry, this is not implemented yet." << std::endl;
        }
        else if (mode == "help")
        {
            if (parser.Specifiers().empty() && parser.Values().empty())
            {
                std::cout << "help" << std::endl <<
                          "help --list" << std::endl <<
                          "help [command]" << std::endl;
            }
            else if (parser.Specifiers().size() == 1 && parser.Values().empty())
            {
                const auto& spec = parser.Specifiers()[0];
                if (spec.Name == "list" && !spec.Value.has_value())
                {
                    std::cout << "Commands: " << std::endl <<
                              "insert: Insert items via name and sterilized format" << std::endl <<
                              "load: Load all objects & time" << std::endl <<
                              "display: Display current elements" << std::endl <<
                              "test: Full test (clear & update for different counts)" << std::endl <<
                              "reset: Remove all current objects" << std::endl <<
                              "lookup: Lookup an entry based on name or key" << std::endl <<
                              "remove: Remove an entry based on name or key" << std::endl <<
                              "help: Get help on a command, or list commands" << std::endl <<
                              "quit: Save & Quit" << std::endl << std::endl;
                }
                else
                    std::cout << "Unrecognized specifier '" << spec.Name << "', or there was a value supplied (none expected)" << std::endl;
            }
            else if (parser.Specifiers().empty() && parser.Values().size() == 1)
            {
                const auto& command = parser.Values()[0].Value;
                if (command == "insert")
                    std::cout << "insert [name] [value]" << std::endl;
                else if (command == "load")
                    std::cout << "load" << std::endl;
                else if (command == "display")
                    std::cout << "display" << std::endl;
                else if (command == "test")
                    std::cout << "test" << std::endl;
                else if (command == "reset")
                    std::cout << "reset" << std::endl;
                else if (command == "lookup")
                    std::cout << "lookup [name]" << std::endl <<
                              "lookup --key [entryKey]" << std::endl;
                else if (command == "remove")
                    std::cout << "remove [name]" << std::endl <<
                              "remove --key [entryKey]" << std::endl;
                else if (command == "help")
                    std::cout << "help" << std::endl <<
                              "help --list" << std::endl <<
                              "help [command]" << std::endl;
                else if (command == "quit")
                    std::cout << "quit" << std::endl;
                else
                    std::cout << "Unrecognized command name '" << command << '\'' << std::endl;
            }
            else //Too many
                std::cout << "Too many values passed into the help command. Help expects one value or one specifier" << std::endl;
        }
        else if (mode.empty())
            continue;
        else
            std::cout << "Unrecognized command: \'" << mode << "\'\n";
    }
    (void)New->Save();
}