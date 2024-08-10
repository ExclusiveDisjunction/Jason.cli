//
// Created by exdisj on 8/8/24.
//

#ifndef JASON_PACKAGEUTILITY_H
#define JASON_PACKAGEUTILITY_H

#include <string>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>

#include "PackageEntry.h"

struct PackageHandle
{
public:
    explicit PackageHandle(std::string path, std::ios::openmode flags = (std::ios::out | std::ios::in));
    PackageHandle(const PackageHandle& obj) = delete;
    PackageHandle(PackageHandle&& obj) noexcept;
    ~PackageHandle();

    void Close() noexcept;

    std::string path;
    std::fstream file;
};

struct PreProcessedPackage
{
public:
    explicit PreProcessedPackage(PackageHandle& handle) : handle(std::move(handle)) {}
    PreProcessedPackage(const PreProcessedPackage& obj) = delete;

    PackageHandle handle;

    std::streampos header, links, entries, functions;
};

class IndexedEntry
{
public:
    IndexedEntry() : beg(std::streampos()), end(std::streampos()), name(std::string()), type(PackageEntryType::Temporary), load_imm(false) {}
    IndexedEntry(std::streampos beg, std::streampos end, std::string name, PackageEntryType type, bool load_imm) : beg(beg), end(end), name(std::move(name)), type(type), load_imm(load_imm) {}

    std::streampos beg, end;
    std::string name;
    PackageEntryType type;
    bool load_imm;
};
class IndexedPackage
{
public:
    IndexedPackage(PackageHandle& handle, std::vector<IndexedEntry> entries) : handle(std::move(handle)), entries(std::move(entries)) {}
    IndexedPackage(const IndexedPackage& obj) = delete;

    PackageHandle handle;
    std::vector<IndexedEntry> entries;
    std::streampos headerLoc, linksLoc, entriesLoc, functionsLoc;
};

class UnloadedPackage
{
public:
    UnloadedPackage(PackageHandle& handle, unsigned long PackageID) : handle(std::move(handle)), ID(PackageID) {}
    UnloadedPackage(const UnloadedPackage& obj) = delete;

    PackageHandle handle;
    unsigned long ID;
};

class Version
{
public:
    Version() : Version(0, 0, 0) {}
    Version(unsigned Major, unsigned Minor, unsigned Release) : Major(Major), Minor(Minor), Release(Release) {}

    unsigned Major;
    unsigned Minor;
    unsigned Release;
};
std::ostream& operator<<(std::ostream& out, const Version& obj) noexcept;
std::istream& operator>>(std::istream& in, Version& obj);

#endif //JASON_PACKAGEUTILITY_H
