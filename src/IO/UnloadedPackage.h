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
    UnloadedPackage(PackageHandle&& handle, unsigned long PackageID);
    UnloadedPackage(const UnloadedPackage& obj) = delete;

    PackageHandle handle;
    unsigned long PackID;
    std::string name;
};

#endif //JASON_UNLOADEDPACKAGE_H
