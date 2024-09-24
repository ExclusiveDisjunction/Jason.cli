//
// Created by exdisj on 8/7/24.
//

#include "Session.h"

#include "../Common.h"
#include "UnloadedPackage.h"

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

    auto rawUsr = FindUserPackage();
    if (!rawUsr)
    {
        std::cerr << "Locating Error: Could not locate the `usr` package." << std::endl;
        return false;
    }

    auto* tree = new PackageLinkTree();
    if (!GetLinksTree(*tree, std::move(*rawUsr)))
    {
        std::cerr << "Pre Processing Error: Could not load the link tree." << std::endl;
        delete tree;
        return false;
    }

    std::vector<PreProcessedPackage*> preProc;
    tree->Flatten(preProc);
    delete tree;

    //Clean up packages that are not to be loaded.
    {
        for (auto& curr : preProc)
        {
            if (!curr->handle.IsEnabled())
            {
                this->unloadedPackages.push_back(new UnloadedPackage(std::move(curr->handle.GetHost()), GetNextID()));
                delete curr;
                curr = nullptr;

                //Further functions that use the array will skip over nullptr items.
            }
        }
    }

    if (!InflatePackages(preProc))
    {
        std::cerr << "Inflating Error: Could not complete inflation phase." << std::endl;

        for (auto& item : preProc)
        {
            delete item;
            item = nullptr;
        }
        preProc.clear();

        return false;
    }

    //Clean up pre-processed packages.
    {
        for (auto& item : preProc)
        {
            delete item;
            item = nullptr;
        }
        preProc.clear();
    }

    if (!IndexPackageEntries()) //Note that if failed, IndexPackageEntries will clean up both indexed and preProc. preProc is cleaned up no matter what.
    {
        std::cerr << "Indexing Error: Could not complete indexing phase." << std::endl;
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

std::optional<FileHandle> Session::FindUserPackage()
{
    try
    {
        std::string path = std::string(getenv("HOME")) + "/usr.jason";
        return FileHandle(path);
    }
    catch (...)
    {
        return {};
    }
}

bool Session::GetLinksTree(PackageLinkTree& tree, FileHandle&& usr)
{
    std::optional<PackageIndex> index = GetPackageIndex(usr);
    if (!index)
        return false;

    tree.InitiateRoot(std::move(usr), true, *index);
    return FillLinkTree(tree.GetRoot(), tree);
}
bool Session::FillLinkTree(PackageLinkNode& parent, const PackageLinkTree& tree) noexcept
{
    std::vector<PackageLink> links;
    if (!ExtractLinks(parent, links))
        return false;

    for (auto& link : links)
    {
        if (tree.Contains(link.GetPath()))
            continue;

        std::optional<PackageIndex> index = GetPackageIndex(link.GetHost());
        if (!index)
            return false;

        PackageLinkNode& created = parent.AddChild(std::move(link), *index);
        if (!FillLinkTree(created, tree))
            return false;
    }

    return true;
}
std::optional<PackageIndex> Session::GetPackageIndex(PackageHandle& target) noexcept
{
    PackageIndex index;

    std::fstream& file = target.file;
    if (!file)
        return {};

    file.seekg(0, std::ios::beg);

    std::string curr;
    file >> curr;

    if (curr != "jason")
    {
        std::cerr << "Processing package at '" << target.path << "' error: File is not of jason format.";
        return {};
    }
    index.header = file.tellg();

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

    if (!findLocation(target, index.links, "links") || !findLocation(target, index.entries, "entry"), !findLocation(target, index.functions, "functions"))
        return {};
    else
        return index;
}
bool Session::ExtractLinks(PackageLinkNode& target, std::vector<PackageLink>& result)
{
    std::fstream& file = target.GetLink().GetHost().file;
    const PackageIndex& index = target.GetIndex();
    if (!file)
        return false;

    file.seekg(index.links);
    std::string curr;
    file >> curr;
    if (curr != "links" || !file)
        return false;

    std::getline(file, curr); //Remove whitespace before statement
    while (file.tellg() < index.entries)
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
            std::getline(ss, part);
            trim(part);
            std::filesystem::path link_target(part);

            result.emplace_back(PackageHandle(link_target), enabled);
        }
        catch (std::logic_error& e)
        {
            std::cerr << "Pre Processing Error: The package at '" << target.GetLink().GetPath().string() << "' could not be processed because one of its links could not be located." << std::endl;
            return false;
        }
    }

    return true;
}

bool Session::InflatePackages(std::vector<PreProcessedPackage*>& indexed)
{
    for (auto& item : indexed)
    {
        if (!item)
            continue;

        auto inflated = InflatePackage(item);
        if (!inflated)
            return false;

        this->packages.push_back(inflated);
    }

    return true;
}
Package* Session::InflatePackage(PreProcessedPackage*& target)
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
        auto linksD = directory / "links";
        PackageHandle links(linksD, std::ios::out | std::ios::trunc);

        std::fstream& host = target->handle.GetHost().file;
        if (!host)
            throw std::logic_error("Package Inflation Error: Could not read from host package.");

        host.seekg(target->index.links);
        {
            std::string curr;
            host >> curr; //This skips over the "links" part.
        }
        readFileBlock(links.file, host, target->index.entries); //Reads links. Remember that the header is not considered.

        Package* result;
        try
        {
            //Since the files were opened in write-only mode, we have to re-open them in read/write mode.
            links.file.close();

            links.file.open(links.path);
            if (!links.file)
                throw std::logic_error("Could not re-open inflated files");

            result = new Package(directory, std::move(target->handle.GetHost()), std::move(links), ThisID, target->index);
        }
        catch (std::logic_error& e)
        {
            std::cerr << "Package Inflation Error: Could not inflate package entries, header, or links because of '" << e.what() << "'." << std::endl;
            result = nullptr;
        }

        return result;
    }
    catch (std::logic_error& e)
    {
        std::cerr << "Package Inflation Error: Inflated files could not be created. Message: " << e.what() << std::endl;
        return nullptr;
    }
}

bool Session::IndexPackageEntries()
{
    bool ok = true;
    for (auto& pack : packages)
        ok &= pack->IndexEntries();

    return ok;
}

std::optional<unsigned long> Session::GetPackageID(const std::string& name) const noexcept
{

}
std::optional<PackageEntryKey> Session::ResolveEntry(const std::string& name) const noexcept
{
    auto loc = name.find("::");
    std::string packName, entryName;
    if (loc <= name.size())
    {
        packName = name.substr(0, loc);
        entryName = name.substr(loc + 1);
    }
    else
    {
        packName = "usr";
        entryName = name;
    }

    auto packIter = ResolvePackage(packName);
    if (packIter != packages.end())
        return (*packIter)->ResolveEntry(entryName);
    else
        return {};
}

bool Session::IsPackageLoaded(unsigned long ID) const noexcept
{
    return ResolvePackage(ID) != this->packages.end();
}
bool Session::LoadPackage(unsigned long ID) noexcept
{

}
bool Session::UnloadPackage(unsigned long ID) noexcept
{

}

std::vector<Package*>::const_iterator Session::ResolvePackage(const std::string& name) const noexcept
{
    return std::find_if(this->packages.begin(), this->packages.end(), [&name](Package* pack) -> bool
    {
        return pack->name == name;
    });
}
std::vector<UnloadedPackage*>::const_iterator Session::ResolveUnloadedPackage(const std::string& name) const noexcept
{
    return std::find_if(this->unloadedPackages.begin(), this->unloadedPackages.end(), [&name](UnloadedPackage* pack) -> bool
    {
        return pack->name == name;
    });
}
std::vector<Package*>::const_iterator Session::ResolvePackage(unsigned long ID) const noexcept
{
    return std::find_if(this->packages.begin(), this->packages.end(), [&ID](Package* pack) -> bool
    {
        return pack->PackID == ID;
    });
}
std::vector<Package*>::iterator Session::ResolvePackage(unsigned long ID) noexcept
{
    return std::find_if(this->packages.begin(), this->packages.end(), [&ID](Package* pack) -> bool
    {
        return pack->PackID == ID;
    });
}
std::vector<UnloadedPackage*>::const_iterator Session::ResolveUnloadedPackage(unsigned long ID) const noexcept
{
    return std::find_if(this->unloadedPackages.begin(), this->unloadedPackages.end(), [&ID](UnloadedPackage* pack) -> bool
    {
        return pack->PackID == ID;
    });
}
std::vector<UnloadedPackage*>::iterator Session::ResolveUnloadedPackage(unsigned long ID) noexcept
{
    return std::find_if(this->unloadedPackages.begin(), this->unloadedPackages.end(), [&ID](UnloadedPackage* pack) -> bool
    {
        return pack->PackID == ID;
    });
}