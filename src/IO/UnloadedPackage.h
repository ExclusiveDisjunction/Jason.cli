//
// Created by exdisj on 8/14/24.
//

#ifndef JASON_UNLOADEDPACKAGE_H
#define JASON_UNLOADEDPACKAGE_H

#include "PackageHandle.h"
#include <string>

class UnloadedPackage
{
public:
    UnloadedPackage(std::filesystem::path target, unsigned long PackageID, std::string name);

    std::filesystem::path target;
    unsigned long PackID;
    std::string name;
};

#endif //JASON_UNLOADEDPACKAGE_H
