//
// Created by exdisj on 8/7/24.
//

#ifndef JASON_PACKAGEENTRYKEY_H
#define JASON_PACKAGEENTRYKEY_H

#include <iostream>

class PackageEntryKey
{
public:
    PackageEntryKey() : PackageEntryKey(0, 0) {}
    PackageEntryKey(unsigned long PackageID, unsigned long EntryID) noexcept : PackageID(PackageID), EntryID(EntryID) {}

    unsigned long PackageID;
    unsigned long EntryID;

    bool operator<(const PackageEntryKey& obj) const noexcept;
    bool operator<=(const PackageEntryKey& obj) const noexcept;
    bool operator>(const PackageEntryKey& obj) const noexcept;
    bool operator>=(const PackageEntryKey& obj) const noexcept;
    bool operator==(const PackageEntryKey& obj) const noexcept;
    bool operator!=(const PackageEntryKey& obj) const noexcept;
};

std::ostream& operator<<(std::ostream& obj, const PackageEntryKey& key) noexcept;

template <>
struct std::hash<PackageEntryKey>
{
  std::size_t operator()(const PackageEntryKey& k) const
  {
    using std::size_t;
    using std::hash;
    using std::string;

    // Compute individual hash values for first,
    // second and third and combine them using XOR
    // and bit shifting:

    return hash<unsigned long>()(k.PackageID) ^ (hash<unsigned long>()(k.EntryID) << 1);
  }
};


#endif //JASON_PACKAGEENTRYKEY_H
