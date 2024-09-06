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

#include "PackageEntryKey.h"
#include "PackageEntry.h"
#include "PackageUtility.h"
#include "PackageHandle.h"
#include "PackageHeader.h"

class Package
{
private:
    enum State
    {
        None = 0,
        ReadOnly = 1
    };

    Package(std::filesystem::path location, FileHandle&& index, FileHandle&& header_l, unsigned long ID, std::string name, PackageHeader&& header);

    std::filesystem::path location;
    FileHandle index_l;
    FileHandle header_l;
    
    unsigned long packID;
    unsigned long currID = 0;
    unsigned char state = 0;

    std::string name;
    PackageHeader header;

    std::vector<PackageEntry*> entries;

    [[nodiscard]] bool IndexEntries();

    [[nodiscard]] const PackageEntry* GetEntry(unsigned long ID) const noexcept;
    [[nodiscard]] PackageEntry* GetEntry(unsigned long ID) noexcept;

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

    [[nodiscard]] bool Compress(std::ostream& out) noexcept;
    [[nodiscard]] bool WriteIndex(std::ostream& out) const noexcept;
    [[nodiscard]] bool WriteHeader(std::ostream& out) const noexcept;
    void Close() noexcept;

    std::optional<PackageEntryKey> ResolveEntry(std::string& name) noexcept;
    const PackageEntry& ResolveEntry(PackageEntryKey key) const;
    PackageEntry& ResolveEntry(PackageEntryKey key);

    bool RemoveEntry(unsigned long ID) noexcept; //Removes from the internal list & deletes it.
    bool ReleaseEntry(unsigned long ID) noexcept; //Removes from the internal list, but does not delete it.
    bool SwapEntry(unsigned long ID, PackageEntry* newItem) noexcept;
    std::optional<PackageEntryKey> AddEntry(std::string name, PackageEntryType type, VariableType* data) noexcept;

    bool DoesEntryExist(unsigned long ID) noexcept;
    const VariableType& GetEntryValue(unsigned long ID);
    bool SetEntryValue(unsigned long ID, VariableType* Data) noexcept;

};

#endif //JASON_PACKAGE_H
