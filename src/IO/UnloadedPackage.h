//
// Created by exdisj on 8/14/24.
//

#ifndef JASON_UNLOADEDPACKAGE_H
#define JASON_UNLOADEDPACKAGE_H

#include "FileHandle.h"
#include <string>

class UnloadedPackage
{
public:
    UnloadedPackage(const class Package& obj) noexcept;
    UnloadedPackage(std::filesystem::path target, unsigned long PackageID, std::string name) noexcept;

    std::filesystem::path Target;
    unsigned long PackID;
    std::string Name;
};

#endif //JASON_UNLOADEDPACKAGE_H
