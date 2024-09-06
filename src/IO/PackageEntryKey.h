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
    PackageEntryKey(const PackageEntryKey& obj) = default;
    PackageEntryKey(PackageEntryKey&& obj) noexcept = default;

    unsigned long PackageID;
    unsigned long EntryID;
};


#endif //JASON_PACKAGEENTRYKEY_H
