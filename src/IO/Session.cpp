//
// Created by exdisj on 8/7/24.
//

#include "Session.h"

#include "../Common.h"

#include <filesystem>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>

Session::Session() : packages(), unloadedPackages(), currID(0) {}
Session::~Session()
{
    ForceClose();

    for (Package*& item : packages)
        delete item;
    for (UnloadedPackage*& item : unloadedPackages)
        delete item;

    packages.clear();
    unloadedPackages.clear();
}

bool Session::Initiate()
{
    if (this->initiated)
        return true;

    std::optional<PackageHandle> rawUsr = FindUserPackage();
    if (!rawUsr)
    {
        std::cerr << "Indexing Error: Could not locate the `usr` package." << std::endl;
        return false;
    }

    std::vector<PreProcessedPackage*> preProc;
    if (!PreProcessPackages(*rawUsr, preProc))
    {
        for (PreProcessedPackage*& item : preProc)
            delete item;
        preProc.clear();

        std::cerr << "Pre Processing Error: Could not complete pre-processing phase." << std::endl;
        return false;
    }

    std::vector<IndexedPackage*> indexed;
    if (!IndexPackages(preProc, indexed)) //Note that if failed, IndexPackages will clean up both indexed and preProc. preProc is cleaned up no matter what.
    {
        std::cerr << "Indexing Error: Could not complete indexing phase." << std::endl;
        return false;
    }

    if (!InflatePackages(indexed))
    {
        std::cerr << "Inflating Error: Could not complete inflation phase." << std::endl;
        return false;
    }

    if (!LoadInflatedEntries())
    {
        std::cerr << "Loading Error: Could not load the variables marked with 'Load Immediately'." << std::endl;
        return false;
    }

    this->initiated = true;
    return true;
}
bool Session::CheckForUnclosedPackages() noexcept
{
    auto directory = std::filesystem::temp_directory_path() / "jason";
    if (std::filesystem::exists(directory) && !std::filesystem::is_empty(directory))
    {
        std::cout << "It looks like Jason was not able to close properly, leaving some packages open.\n";
        while (true)
        {
            std::cout << "Would you like to (1) Start Jason with these open packages, (2) Exit Jason, (3) Startup as normal? (1, 2, or 3): ";
            int input;
            std::cin >> input;
            switch (input)
            {
                case 1:
                {
                    try
                    {
                        this->LoadFromPreviousPackages(directory);
                        std::cout << "Session successfully rebuilt.\n";
                        this->initiated = true;
                        return true;
                    }
                    catch (std::logic_error& e)
                    {
                        std::cout << "Re-building Error: Could not reconstruct from previous state because of '" << e.what() << "'\n";
                        break;
                    }
                }
                case 2:
                    return false;
                case 3:
                {
                    std::filesystem::remove_all(directory);
                    std::cout << "Previous session successfully cleared.\n";
                    return true;
                }
                default:
                    std::cout << "Option not understood.\n";
                    break;
            }
        }
    }
    else
        return true;
}

void Session::LoadFromPreviousPackages(const std::filesystem::path& hostDir)
{
    throw std::logic_error("Not Implemented");
}

bool Session::Save(bool DeleteDir) noexcept
{
    bool result = true;
    for (Package*& pack : packages)
    {
        if (!pack->Save())
        {
            result = false;
            std::cout << "Session Saving: Failed to save package at '" << pack->pack.path << "'.\n";
        }
        else
        {
            if (DeleteDir)
            {
                pack->Close();
                std::filesystem::remove_all(pack->dir_path);
            }
        }
    }

    return result;
}
bool Session::Close()
{
    if (!this->initiated)
        return true;

    std::cout << "Saving session...\n";
    if (!Save(true))
    {
        std::cout << "Session save failed. Would you still like to exit Jason? (y/n): ";
        char c;
        std::cin >> c;

        return false;
    }
    else
        std::cout << "Session is successfully saved.\n";

    this->initiated = false;
    return true;
}
void Session::ForceClose()
{
    if (this->initiated)
        (void)Save(true);

    std::filesystem::remove_all(std::filesystem::temp_directory_path() / "jason");

    this->initiated = false;
}

std::optional<PackageHandle> Session::FindUserPackage()
{
    try
    {
        std::string path = std::string(getenv("HOME")) + "/usr.jason";
        return PackageHandle(path);
    }
    catch (...)
    {
        return {};
    }
}

bool Session::PreProcessPackages(PackageHandle& usr, std::vector<PreProcessedPackage*>& result)
{
    PreProcessedPackage* usr_proc = PreProcessPackage(usr);
    if (!usr_proc)
    {
        std::cerr << "Pre Processing Error: The package at '" << usr.path << "' could not be pre-processed." << std::endl;
        return false;
    }

    result.push_back(usr_proc);

    //NOTICE: The memory for this function is handled by the caller function. Therefore, this function can return without deleting usr_proc, and it will not cause a memory leak.

    std::vector<std::pair<PackageHandle, bool>> FoundTargets;
    if (!ExtractLinks(*usr_proc, FoundTargets))
    {
        std::cerr << "Pre Processing Error: The package at '" << usr.path << "' could not be pre-processed because of linker errors." << std::endl;
        return false;
    }

    for (auto& target : FoundTargets)
    {
        auto searchResult = std::find_if(result.begin(), result.end(), [&target](PreProcessedPackage* curr) -> bool
        {
            return curr->handle.path == target.first.path;
        });

        if (searchResult != result.end()) //Already pre-processed, skip
            continue;
        else
        {
            //Two cases: 1) To load, so call this function again and load it, or 2) Do not load so just finish pre-processing and move on. We will assume the file is of proper format until it is later loaded.
            if (target.second) //we load it
            {
                if (!PreProcessPackages(target.first, result))
                {
                    std::cerr << "Pre Processing Error: The package at '" << usr.path << "' could not be pre-processed because the linked package '" << target.first.path << "' failed pre-processing." << std::endl;
                    return false;
                }
            }
            else
            {
                this->unloadedPackages.push_back(new UnloadedPackage(target.first, GetNextID()));
            }
        }
    }

    return true;
}
PreProcessedPackage* Session::PreProcessPackage(PackageHandle& target)
{
    std::streampos header, links, entries, functions;

    std::fstream& file = target.file;
    if (!file)
        return nullptr;

    file.seekg(0, std::ios::beg);

    std::string curr;
    file >> curr;

    if (curr != "jason")
    {
        std::cerr << "Processing package at '" << target.path << "' error: File is not of jason format.";
        return nullptr;
    }
    header = file.tellg();

    auto findLocation = [](PackageHandle& package, std::streampos& target, const std::string& name) -> bool
    {
        std::string curr;
        while (curr != name && !package.file.eof())
        {
            target = package.file.tellg();
            package.file >> curr;
        }
        if (curr != name) //EOF reached, but item was not found.
        {
            std::cerr << "Processing package at '" << package.path << "' error: File is not of jason format (no links section).";
            return false;
        }

        return true;
    };

    if (!findLocation(target, links, "links") || !findLocation(target, entries, "entry"), !findLocation(target, functions, "functions"))
        return nullptr;
    else
    {
        auto* result = new PreProcessedPackage(target);
        result->header = header;
        result->links = links;
        result->entries = entries;
        result->functions = functions;

        return result;
    }
}

bool Session::ExtractLinks(PreProcessedPackage& target, std::vector<std::pair<PackageHandle, bool>>& result)
{
    std::fstream& file = target.handle.file;
    if (!file)
        return false;

    result.clear();

    file.seekg(target.links);
    std::string curr;
    file >> curr;
    if (curr != "links" || !file)
        return false;

    std::getline(file, curr); //Remove whitespace before statement
    while (file.tellg() < target.entries)
    {
        std::getline(file, curr);
        if (!file || file.eof())
            return false;

        std::stringstream ss(curr);

        std::string part;
        ss >> part;
        if (part != "link")
            return false;

        try
        {
            ss >> part;
            bool enabled = part == "enabled";
            ss >> part;
            result.emplace_back(PackageHandle(part), enabled);
        }
        catch (...)
        {
            return false;
        }
    }

    return true;
}

bool Session::IndexPackages(std::vector<PreProcessedPackage*>& toIndex, std::vector<IndexedPackage*>& output)
{
    bool ok = true;
    for (auto & i : toIndex)
    {
        IndexedPackage* result = IndexPackage(i);
        if (!result)
        {
            ok = false;
            break;
        }
        output.push_back(result);
    }

    for (auto*& item : toIndex)
        delete item;
    toIndex.clear();

    if (!ok)
    {
        for (auto*& item : output)
            delete item;
        output.clear();
    }

    return ok;
}
IndexedPackage* Session::IndexPackage(PreProcessedPackage*& toIndex)
{
    std::vector<IndexedEntry> entries;

    //TODO: Indexing

    auto* result = new IndexedPackage(toIndex->handle, entries);
    result->headerLoc = toIndex->header;
    result->linksLoc = toIndex->links;
    result->entriesLoc = toIndex->entries;
    result->functionsLoc = toIndex->functions;

    delete toIndex;
    toIndex = nullptr;
    return result;
}

bool Session::InflatePackages(std::vector<IndexedPackage*>& indexed)
{
    bool ok = true;
    for (auto& item : indexed)
    {
        auto inflated = InflatePackage(item);
        if (!inflated)
        {
            ok = false;
            break;
        }

        this->packages.push_back(inflated);
    }

    if (!indexed.empty())
    {
        for (auto& item : indexed)
            delete item;
        indexed.clear();
    }

    return ok;
}
Package* Session::InflatePackage(IndexedPackage*& target)
{
    if (!target)
        return nullptr;

    //Inflation will take the individual parts of the file and put them into different paths.
    unsigned long ThisID = this->GetNextID();
    auto directory = std::filesystem::temp_directory_path() / "jason" / std::to_string(ThisID);

    if (!std::filesystem::exists(directory) && !std::filesystem::create_directories(directory))
    {
        std::cerr << "Package Inflation Error: Could not create directory for inflated package at '" << directory << "'." << std::endl;
        return nullptr;
    }

    try
    {
        //Create the 'source' file for reconstruction, if needed.
        {
            std::ofstream source(directory / "source");
            if (!source)
                throw std::string("Package Inflation Error: Could not create the 'source' file.");
            source << target->handle.path;
            source.close();
        }

        auto headerD = directory / "header", linksD = directory / "links";
        PackageHandle header(headerD, std::ios::out | std::ios::trunc), links(linksD, std::ios::out | std::ios::trunc);

        std::fstream& host = target->handle.file;
        if (!host)
            throw std::string("Package Inflation Error: Could not read from host package.");


        host.seekg(target->headerLoc);
        readFileBlock(header.file, host, target->linksLoc); //Reads header

        host.seekg(target->linksLoc);
        {
            std::string curr;
            host >> curr; //This skips over the "links" part.
        }
        readFileBlock(links.file, host, target->entriesLoc); //Reads links.

        Package* result;
        try
        {
            //Since the files were opened in write-only mode, we have to re-open them in read/write mode.
            links.file.close();
            header.file.close();

            links.file.open(links.path);
            header.file.open(header.path);
            if (!links.file || !header.file)
                throw std::string("Package Inflation Error: Could not re-open inflated files.");

            result = new Package(directory, target->handle, links, header, ThisID);
        }
        catch (std::logic_error& e)
        {
            std::cerr << "Package Inflation Error: Could not inflate package entries, header, or links because of '" << e.what() << "'." << std::endl;
            result = nullptr;
        }

        delete target;
        target = nullptr;
        return result;
    }
    catch (std::string& s)
    {
        std::cerr << s << std::endl;
        delete target;
        target = nullptr;
        return nullptr;
    }
    catch (std::logic_error& e)
    {
        std::cerr << "Package Inflation Error: Inflated files could not be created. Message: " << e.what() << std::endl;
        return nullptr;
    }
}

bool Session::LoadInflatedEntries()
{
    bool result = true;
    for (Package*& pack : packages)
        result &= pack->LoadEntries(false);

    return result;
}

std::optional<unsigned long> Session::GetPackageID(const std::string& name) const noexcept
{

}
std::optional<PackageEntryKey> Session::ResolveEntry(const std::string& name) const noexcept
{

}

bool Session::LoadPackage(unsigned long ID)
{

}
bool Session::UnloadPackage(unsigned long ID)
{

}

const Package* Session::ResolvePackage(const std::string& name) const noexcept
{

}
Package* Session::ResolvePackage(const std::string& name) noexcept
{

}