//
// Created by Hollan on 10/2/24.
//

#include "PackageEntryKey.h"

bool PackageEntryKey::operator<(const PackageEntryKey& obj) const noexcept
{
    return this->PackageID <= obj.PackageID && this->EntryID < obj.EntryID;
}
bool PackageEntryKey::operator<=(const PackageEntryKey& obj) const noexcept
{
    return *this == obj || *this < obj;
}
bool PackageEntryKey::operator>(const PackageEntryKey& obj) const noexcept
{
    return this->PackageID >= obj.PackageID && this->EntryID > obj.EntryID;
}
bool PackageEntryKey::operator>=(const PackageEntryKey& obj) const noexcept
{
    return *this == obj || *this > obj;
}
bool PackageEntryKey::operator==(const PackageEntryKey& obj) const noexcept 
{
    return this->PackageID == obj.PackageID && this->EntryID == obj.EntryID;
}
bool PackageEntryKey::operator!=(const PackageEntryKey& obj) const noexcept 
{
    return this->PackageID != obj.PackageID || this->EntryID != obj.EntryID;
}

std::ostream& operator<<(std::ostream& obj, const PackageEntryKey& key) noexcept
{
    obj << key.PackageID << '.' << key.EntryID;
    return obj;
}