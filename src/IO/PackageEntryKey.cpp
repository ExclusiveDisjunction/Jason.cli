//
// Created by Hollan on 10/2/24.
//

#include "PackageEntryKey.h"

std::ostream& operator<<(std::ostream& obj, const PackageEntryKey& key) noexcept
{
    obj << key.PackageID << '.' << key.EntryID;
    return obj;
}