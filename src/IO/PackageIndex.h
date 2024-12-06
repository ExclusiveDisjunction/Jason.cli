#ifndef JASON_PACKAGEINDEX_H
#define JASON_PACKAGEINDEX_H

#include <iostream>
#include <utility>
#include <fstream>
#include <vector>

#include "FileHandle.h"
#include "PackageEntryIndex.h"

class PackageEntry;

//Serves to store the package's index, by listing off its iterators. 
class PackageIndex 
{
private:
    FileHandle handle;

public:
    PackageIndex(FileHandle&& handle);

    std::vector<PackageEntryIndex> ReadIndex(unsigned long PackageID) noexcept;
    bool Write(const std::vector<PackageEntry>& entries) noexcept;
    static bool Write(std::ostream& out, const std::vector<PackageEntry>& entries) noexcept;

    void Close();
    void TruncateFile();
};

#endif