//
// Created by Hollan on 8/21/24.
//

#ifndef JASON_PACKAGEHEADER_H
#define JASON_PACKAGEHEADER_H

#include <string>
#include <iostream>
#include <optional>

#include "PackageHandle.h"

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

class PackageHeader
{
public:
    [[nodiscard]] static std::optional<PackageHeader> FromFile(std::istream& in, std::optional<std::streamoff> endLoc = {});
    PackageHeader(std::optional<PackageHandle>&& handle, Version ver, std::optional<std::string> author = {}, bool locked = false);

    std::optional<PackageHandle> handle;
    Version ver;
    std::optional<std::string> author;
    bool locked;

    void Output(std::ostream& out) const noexcept;
};


#endif //JASON_PACKAGEHEADER_H
