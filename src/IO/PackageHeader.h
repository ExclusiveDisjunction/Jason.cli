//
// Created by Hollan on 8/21/24.
//

#ifndef JASON_PACKAGEHEADER_H
#define JASON_PACKAGEHEADER_H

#include <string>
#include <iostream>
#include <optional>

#include "FileHandle.h"

class Version
{
public:
    Version() : Version(0, 0, 0) {}
    Version(unsigned Major, unsigned Minor, unsigned Release) : Major(Major), Minor(Minor), Release(Release) {}

    unsigned Major;
    unsigned Minor;
    unsigned Release;

    bool operator==(const Version& obj) const noexcept;
    bool operator!=(const Version& obj) const noexcept;
};
std::ostream& operator<<(std::ostream& out, const Version& obj) noexcept;
std::istream& operator>>(std::istream& in, Version& obj);

class PackageHeader
{
private:
    FileHandle handle;
    Version version;
    std::optional<std::string> author;
    bool readonly;

    [[nodiscard]] bool Write();
    template<typename T>
    [[nodiscard]] bool SetValue(T& target, const T& New)
    {
        if (target != New)
        {
            target = New;
            return Write();
        }
        else
            return true;
    }

public:
    PackageHeader(FileHandle&& handle, Version ver, std::optional<std::string>&& author = {}, bool readonly = false);

    void Close();

    friend std::ostream& operator<<(std::ostream&, const PackageHeader&);
    friend std::istream& operator>>(std::istream&, PackageHeader&);

    [[nodiscard]] const std::filesystem::path& GetLocation() const noexcept;

    [[nodiscard]] const Version& GetVersion() const noexcept;
    [[nodiscard]] bool SetVersion(Version New) noexcept;

    [[nodiscard]] const std::optional<std::string>& GetAuthor() const noexcept;
    [[nodiscard]] bool SetAuthor(std::optional<std::string> New) noexcept;

    [[nodiscard]] bool IsReadOnly() const noexcept;
    [[nodiscard]] bool SetReadOnly(bool New) noexcept;
};

std::ostream& operator<<(std::ostream& out, const PackageHeader& obj);
std::istream& operator>>(std::istream& in, PackageHeader& obj);


#endif //JASON_PACKAGEHEADER_H
