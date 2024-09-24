#ifndef JASON_PACKAGEINDEX_H
#define JASON_PACKAGEINDEX_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <utility>

#include "FileHandle.h"

class PackageEntry;

//Serves to store the package's index, by listing off its iterators. 
class PackageIndex 
{
private:
    FileHandle handle;

public:
    PackageIndex(FileHandle&& handle);

    bool ReadIndex(std::vector<PackageEntry*>& entries) noexcept;
    bool Write(std::ostream& out, const std::vector<PackageEntry*>& entries) const noexcept;

    void Close();
};

template<typename stream>
class PackageIndexLine
{
private:
    stream& s;
public:
    explicit PackageIndexLine(stream& target) : s(target) { }

    stream& Expose() noexcept { return s; }
};

using PackageIndexReadLine = PackageIndexLine<std::istream>;
using PackageIndexWriteLine = PackageIndexLine<std::ostream>;

#endif