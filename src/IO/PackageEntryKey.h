//
// Created by exdisj on 8/7/24.
//

#ifndef JASON_PACKAGEENTRYKEY_H
#define JASON_PACKAGEENTRYKEY_H

class PackageEntryKey
{
public:
    PackageEntryKey(unsigned long PackageID, unsigned long EntryID) noexcept : PackageID(PackageID), EntryID(EntryID) {}

    const unsigned long PackageID;
    const unsigned long EntryID;
};


#endif //JASON_PACKAGEENTRYKEY_H
