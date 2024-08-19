//
// Created by exdisj on 8/14/24.
//

#include "UnloadedPackage.h"
#include "Session.h"

UnloadedPackage::UnloadedPackage(PackageHandle&& handle, unsigned long PackageID) : handle(std::move(handle)), PackID(PackageID)
{
    this->name = Session::MakePackageName(handle.path);
}