//
// Created by exdisj on 8/14/24.
//

#include "UnloadedPackage.h"
#include "Package.h"

UnloadedPackage::UnloadedPackage(const Package& obj) noexcept : Target(obj.Location()), PackID(obj.GetID()), Name(obj.GetName())
{

}
UnloadedPackage::UnloadedPackage(std::filesystem::path target, unsigned long PackageID, std::string name) noexcept : Target(std::move(target)), PackID(PackageID), Name(std::move(name))
{

}
