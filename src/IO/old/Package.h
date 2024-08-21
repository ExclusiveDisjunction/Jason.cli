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

class Package
{
private:
    Package(std::string dir_path, PackageHandle&& pack, PackageHandle&& links, unsigned long ID, const PackageIndex& index);

    std::filesystem::path dir_path;
    std::string name;
    PackageHandle pack;
    PackageHandle links;
    unsigned long PackID;

    PackageIndex index;

    Version ver;
    std::optional<std::string> author;
    bool locked;

    std::vector<PackageEntry> entries;
    unsigned long CurrID;

    [[nodiscard]] bool IndexEntries();

    std::string GetEntryPath(unsigned long ID) noexcept;

    [[nodiscard]] const PackageEntry* GetEntry(unsigned long ID) const noexcept;
    [[nodiscard]] PackageEntry* GetEntry(unsigned long ID) noexcept;

    unsigned long GetNextID() noexcept { return CurrID++;}

public:
    Package(const Package& obj) = delete;
    Package(Package& obj) noexcept = delete;
    ~Package();

    friend class Session;

    Package& operator=(const Package& obj) = delete;
    Package& operator=(Package&& obj) noexcept = delete;

    [[nodiscard]] bool Save() noexcept;
    [[nodiscard]] bool WriteSchematic() const noexcept;
    void Close() noexcept;

    std::optional<PackageEntryKey> ResolveEntry(std::string& name) noexcept;

    bool LoadEntry(unsigned long ID) noexcept;
    bool UnloadEntry(unsigned long ID) noexcept;
    bool ResetEntry(unsigned long ID) noexcept;
    bool DropEntry(unsigned long ID) noexcept;
    std::optional<PackageEntryKey> AddEntry(std::string name, PackageEntryType type, VariableType* data) noexcept;

    bool DoesEntryExist(unsigned long ID) noexcept;
    const VariableType& GetEntryValue(unsigned long ID);
    bool SetEntryValue(unsigned long ID, VariableType* Data) noexcept;

};




#endif //JASON_PACKAGE_H
