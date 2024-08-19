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
#include <filesystem>

#include "PackageEntry.h"
#include "PackageLink.h"
#include "PackageHandle.h"

class PackageIndex
{
public:
    PackageIndex() : PackageIndex(0, 0, 0, 0) {}
    PackageIndex(std::streampos header, std::streampos links, std::streampos entries, std::streampos functions) : header(header), links(links), entries(entries), functions(functions) {}

    std::streampos header, links, entries, functions;
};

struct PreProcessedPackage
{
public:
    PreProcessedPackage(PackageLink&& handle, const PackageIndex& index) : handle(std::move(handle)), index(index) {}
//    PreProcessedPackage(PackageHandle&& handle, const PackageIndex& index)
    PreProcessedPackage(const PreProcessedPackage& obj) = delete;

    PackageLink handle;
    PackageIndex index;
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
