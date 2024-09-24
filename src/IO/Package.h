//
// Created by exdisj on 8/7/24.
//

#ifndef JASON_PACKAGE_H
#define JASON_PACKAGE_H

#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <optional>
#include <filesystem>

#include "PackageEntry.h"
#include "PackageUtility.h"
#include "FileHandle.h"
#include "PackageHeader.h"
#include "PackageIndex.h"

class Package
{
private:
    enum State
    {
        None = 0,
        Compressed = 1
    };

    Package(std::filesystem::path location, unsigned long ID, PackageHeader&& header, PackageIndex&& index, bool isCompressed = false);

    std::filesystem::path location;

    unsigned long packID;
    unsigned long currID = 0;
    unsigned char state;

    std::string name;
    PackageHeader header;
    PackageIndex index;

    std::vector<PackageEntry*> entries;

    [[nodiscard]] bool IndexEntries();

    [[nodiscard]] std::vector<PackageEntry*>::const_iterator GetEntry(unsigned long ID) const noexcept;
    [[nodiscard]] std::vector<PackageEntry*>::iterator GetEntry(unsigned long ID) noexcept;

    unsigned long GetNextID() noexcept { return currID++;}

public:
    Package(const Package& obj) = delete;
    Package(Package& obj) noexcept = delete;
    ~Package();

    friend class Session;

    Package& operator=(const Package& obj) = delete;
    Package& operator=(Package&& obj) noexcept = delete;

    [[nodiscard]] static Package* OpenFromDirectory(std::filesystem::path& dir, unsigned long ID);
    [[nodiscard]] static Package* OpenFromCompressed(std::filesystem::path& pack, std::filesystem::path& targetDir, unsigned long ID);

    [[nodiscard]] const std::filesystem::path& Location() const noexcept;
    [[nodiscard]] std::filesystem::path VarLocation() const noexcept;
    [[nodiscard]] unsigned long GetID() const noexcept;
    [[nodiscard]] const std::string& GetName() const noexcept;
    [[nodiscard]] bool IsCompressed() const noexcept;
    [[nodiscard]] const PackageHeader& Header() const noexcept;
    [[nodiscard]] PackageHeader& Header() noexcept;

    [[nodiscard]] bool Compress(std::ostream& out) const noexcept;
    void Close() noexcept;

    [[nodiscard]] const PackageEntry& ResolveEntry(const std::string& name) const;
    [[nodiscard]] PackageEntry& ResolveEntry(const std::string& name);
    [[nodiscard]] const PackageEntry& ResolveEntry(PackageEntryKey key) const;
    [[nodiscard]] PackageEntry& ResolveEntry(PackageEntryKey key);
    [[nodiscard]] bool DoesEntryExist(unsigned long ID) noexcept;

    [[nodiscard]] bool RemoveEntry(unsigned long ID) noexcept; //Removes from the internal list & deletes it.
    [[nodiscard]] PackageEntry* ReleaseEntry(unsigned long ID) noexcept; //Removes from the internal list, but does not delete it.
    [[nodiscard]] std::optional<PackageEntryKey> AddEntry(std::string name, PackageEntryType type, VariableType* data) noexcept;
};

#endif //JASON_PACKAGE_H
