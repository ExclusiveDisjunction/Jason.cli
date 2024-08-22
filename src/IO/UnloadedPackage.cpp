//
// Created by exdisj on 8/14/24.
//

#include "UnloadedPackage.h"
#include "Session.h"

UnloadedPackage::UnloadedPackage(std::filesystem::path target, unsigned long PackageID, std::string name) : target(std::move(target)), PackID(PackageID), name(std::move(name))
{

}
