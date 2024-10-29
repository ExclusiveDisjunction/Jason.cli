//
// Created by Hollan on 8/21/24.
//

#ifndef JASON_PACKAGEHEADER_H
#define JASON_PACKAGEHEADER_H

#include <string>
#include <iostream>
#include <optional>

#include "FileHandle.h"
#include "../Core/Result.h"
#include "../Core/Version.h"

class PackageHeader
{
private:
    FileHandle handle;
    Version version;
    std::optional<std::string> author;
    bool readonly;

    bool modified = false;

    template<typename T>
    void SetValue(T& target, const T& New)
    {
        if (target != New)
        {
            target = New;
            modified = true;
        }
    }
    
public:
    PackageHeader(FileHandle&& handle);
    [[nodiscard]] static Result<PackageHeader, std::string> OpenHeader(FileHandle&& handle) noexcept;

    friend std::ostream& operator<<(std::ostream&, const PackageHeader&);
    friend std::istream& operator>>(std::istream&, PackageHeader&);

    [[nodiscard]] bool Write() noexcept;
    [[nodiscard]] bool WriteIfModified() noexcept;
    [[nodiscard]] bool Read() noexcept;
    void Close();

    [[nodiscard]] const std::filesystem::path& GetLocation() const noexcept;

    [[nodiscard]] const Version& GetVersion() const noexcept;
    void SetVersion(Version New) noexcept;

    [[nodiscard]] const std::optional<std::string>& GetAuthor() const noexcept;
    void SetAuthor(std::optional<std::string> New) noexcept;

    [[nodiscard]] bool IsReadOnly() const noexcept;
    void SetReadOnly(bool New) noexcept;
};

std::ostream& operator<<(std::ostream& out, const PackageHeader& obj);
std::istream& operator>>(std::istream& in, PackageHeader& obj);


#endif //JASON_PACKAGEHEADER_H
