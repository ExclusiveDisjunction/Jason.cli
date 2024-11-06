//
// Created by exdisj on 8/7/24.
//

#include "Session.h"

#include "../Core/Common.h"
#include "UnloadedPackage.h"

#include <filesystem>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>

Session::Session(std::filesystem::path location, FileHandle&& header) noexcept : packages(), unloadedPackages(), currID(0), location(std::move(location)), header(std::move(header)) 
{

}
Session::Session(Session&& obj) noexcept : packages(std::move(obj.packages)), unloadedPackages(std::move(obj.unloadedPackages)), currID(std::exchange(obj.currID, 0)), location(std::move(obj.location)), header(std::move(obj.header))
{

}
Session::~Session() noexcept
{
    Shutdown();
}

Session& Session::operator=(Session&& obj) noexcept
{
    Shutdown();

    packages = std::move(obj.packages);
    unloadedPackages = std::move(obj.unloadedPackages);
    location = std::move(obj.location);
    currID = std::exchange(obj.currID, 0);
    header = std::move(obj.header);

    return *this;
}

std::unique_ptr<Session> Session::StartSession(std::filesystem::path host) 
{
    //First we check to see if our session directories is ok.
    std::filesystem::path headerP = host / "header";
    if (!std::filesystem::exists(host) || !std::filesystem::exists(headerP))
        throw std::logic_error("The host directory or the header file does not exist");

    FileHandle header(headerP);

    std::unique_ptr<Session> resultPtr(
        new Session(std::move(host), std::move(header))
    );

    Session& result = *resultPtr;
    for (const auto& dir : std::filesystem::directory_iterator{result.location})
    {
        if (dir.is_directory())
        {
            std::filesystem::path loc = dir.path();
            auto PackResult = Package::OpenFromDirectory(std::move(loc), GetNextID());

            result.packages.emplace_back( 
                std::move(
                    Package::OpenFromDirectory(std::move(loc), GetNextID())
                )
            );
        }
    }

    return resultPtr;
}
bool Session::Save() const noexcept
{
    bool result = true;
    for (const auto& pack : packages)
    {
        if (!pack->Save())
        {
            result = false;
            std::cout << "Session Saving: Failed to save package at '" << pack->location << '\n';
        }
    }

    return result;
}
bool Session::SaveAndClose() noexcept
{
    if (!Save())
        return false;
    
    Shutdown();
    return true;
}
void Session::Shutdown() noexcept
{
    packages.clear();
    unloadedPackages.clear();
}

std::optional<unsigned long> Session::GetPackageID(const std::string& name) const noexcept
{
    auto prevResult = GetPackage(name);
    return prevResult ? prevResult.value()->GetID() : std::optional<unsigned long>();
}
std::optional<std::shared_ptr<Package>> Session::GetPackage(const std::string& name) const noexcept
{
    auto iter = ResolvePackage(name);
    if (iter == packages.end())
        return {};
    else 
        return  *iter;
}

bool Session::IsPackageLoaded(unsigned long ID) const noexcept
{
    return ResolvePackage(ID) != this->packages.end();
}
bool Session::LoadPackage(unsigned long ID) noexcept
{
    auto iter = this->ResolveUnloadedPackage(ID);
    if (iter == this->unloadedPackages.end())
        return false;

    auto& target = *iter;

    try 
    {
        packages.emplace_back( 
            std::move(
                Package::OpenFromUnloaded(target)
            )
        );
        unloadedPackages.erase(iter);
    }
    catch (...)
    {
        return false;
    }

    return true;
}
bool Session::UnloadPackage(unsigned long ID) noexcept
{
    auto iter = ResolvePackage(ID);
    if (iter == packages.end())
        return false;

    return UnloadPackage(*iter);
}
bool Session::UnloadPackage(std::shared_ptr<Package> pack) noexcept
{
    if (!pack)
        return false;

    auto iter = std::find(packages.begin(), packages.end(), pack);
    if (iter == packages.end())
        return false; //Not contained

    UnloadedPackage newPack(pack->location, pack->packID, pack->name);
    pack->Close();

    this->packages.erase(iter);
    this->unloadedPackages.emplace_back(std::move(newPack));

    pack.reset();
    return true;
}

Session::cp_iter Session::ResolvePackage(const std::string& name) const noexcept
{
    return std::find_if(this->packages.begin(), this->packages.end(), [&name](const std::shared_ptr<Package>& pack) -> bool
    {
        return pack->name == name;
    });
}
Session::cp_iter Session::ResolvePackage(unsigned long ID) const noexcept
{
    return std::find_if(this->packages.begin(), this->packages.end(), [&ID](const std::shared_ptr<Package>& pack) -> bool
    {
        return pack->GetID() == ID;
    });
}
Session::p_iter Session::ResolvePackage(unsigned long ID) noexcept
{
    return std::find_if(this->packages.begin(), this->packages.end(), [&ID](const std::shared_ptr<Package>& pack) -> bool
    {
        return pack->GetID() == ID;
    });
}

Session::cu_iter Session::ResolveUnloadedPackage(const std::string& name) const noexcept
{
    return std::find_if(this->unloadedPackages.begin(), this->unloadedPackages.end(), [&name](const UnloadedPackage& pack) -> bool
        {
            return pack.Name == name;
        });
}
Session::cu_iter Session::ResolveUnloadedPackage(unsigned long ID) const noexcept
{
    return std::find_if(this->unloadedPackages.begin(), this->unloadedPackages.end(), [&ID](const UnloadedPackage& pack) -> bool
        {
            return pack.PackID == ID;
        });
}
Session::u_iter Session::ResolveUnloadedPackage(unsigned long ID) noexcept
{
    return std::find_if(this->unloadedPackages.begin(), this->unloadedPackages.end(), [&ID](UnloadedPackage& pack) -> bool
        {
            return pack.PackID == ID;
        });
}

const PackageEntry& Session::ResolveEntry(const std::string& name) const
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
    {
        auto& unpacked = *packIter;
        return unpacked->ResolveEntry(name);
        
    }
    else
        throw std::logic_error("The entry could not be found because the package '" + packName + "' does not exist in the session");
}
PackageEntry& Session::ResolveEntry(const std::string& name)
{
    return const_cast<PackageEntry&>(const_cast<const Session*>(this)->ResolveEntry(name));
}
std::optional<PackageEntryKey> Session::ResolveEntryKey(const std::string& name) const noexcept
{
    try
    {
        const auto& elem = ResolveEntry(name);
        return elem.GetIndex().Key();
    }
    catch(...)
    {
        return {};
    }
    
}