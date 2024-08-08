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

#include "PackageEntryKey.h"
#include "PackageEntry.h"
#include "PackageUtility.h"

class Package
{
private:
    std::string dir_path, name;
    PackageHandle pack;
    PackageHandle links;
    PackageHandle header;
    unsigned long PackID;

    Version ver;
    std::optional<std::string> author;
    bool locked;

    std::vector<PackageEntry> entries;
    unsigned long CurrID;

    std::string GetEntryPath(unsigned long ID) noexcept;

    [[nodiscard]] const PackageEntry* GetEntry(unsigned long ID) const noexcept;
    [[nodsicard]] PackageEntry* GetEntry(unsigned long ID) noexcept;

public:
    Package(const Package& obj) = delete;
    Package(Package& obj) noexcept = delete;
    ~Package();

    Package& operator=(const Package& obj) = delete;
    Package& operator=(Package&& obj) noexcept = delete;

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
