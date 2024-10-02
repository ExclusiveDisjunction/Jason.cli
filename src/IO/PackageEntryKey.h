//
// Created by exdisj on 8/7/24.
//

#ifndef JASON_PACKAGEENTRYKEY_H
#define JASON_PACKAGEENTRYKEY_H

class PackageEntryKey
{
public:
    PackageEntryKey() : PackageEntryKey(0, 0) {}
    PackageEntryKey(unsigned long PackageID, unsigned long EntryID) noexcept : PackageID(PackageID), EntryID(EntryID) {}

    unsigned long PackageID;
    unsigned long EntryID;
};

inline std::ostream& operator<<(std::ostream& obj, const PackageEntryKey& key) noexcept
{
    obj << key.PackageID << '.' << key.EntryID;
    return obj;
}


#endif //JASON_PACKAGEENTRYKEY_H
