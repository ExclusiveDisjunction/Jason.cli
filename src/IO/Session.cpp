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
    Shutdown();
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
bool Session::SaveAndClose()
{
    if (!Save())
        return false;
    
    Shutdown();
    return true;
}
void Session::Shutdown()
{
    packages.clear();
    unloadedPackages.clear();
}

std::optional<unsigned long> Session::GetPackageID(const std::string& name) const noexcept
{

}
std::optional<PackageEntryKey> Session::ResolveEntryKey(const std::string& name) const noexcept
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
        try
        {
            return unpacked->ResolveEntry(name).GetIndex().Key();
        }
        catch(...)
        {
            return {};
        }
        
    }
    else
        return {};
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

    std::optional<std::shared_ptr<Package>> attempted = Package::OpenFromDirectory(target.target, target.PackID);
    if (!attempted.has_value())
        return false;

    unloadedPackages.erase(iter);
    packages.emplace_back(std::move(attempted.value()));
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
            return pack.name == name;
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