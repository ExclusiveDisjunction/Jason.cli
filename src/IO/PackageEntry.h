//
// Created by exdisj on 8/7/24.
//

#ifndef JASON_PACKAGEENTRY_H
#define JASON_PACKAGEENTRY_H

#include <string>
#include <filesystem>
#include <optional>

#include "PackageEntryKey.h"
#include "PackageEntryIndex.h"
#include "../Calc/VariableType.h"
#include "../Core/Result.h"

class Package;

class PackageEntry
{
private:
    std::optional<std::unique_ptr<VariableType>> data = {};
    std::weak_ptr<Package> parent;
    PackageEntryIndex index;
    bool modified = false;

public:
    PackageEntry(PackageEntryIndex&& index, std::weak_ptr<Package> parent);
    PackageEntry(const PackageEntry& obj) = delete;
    PackageEntry(PackageEntry&& obj) = default;
    ~PackageEntry();

    friend class Package;

    PackageEntry& operator=(const PackageEntry& obj) = delete;
    PackageEntry& operator=(PackageEntry&& obj) noexcept = default;

    /// @breif Writes the data of the Entry if it is loaded, fails if otherwise.
    [[nodiscard]] bool WriteData(std::ostream& out) const noexcept;
    [[nodiscard]] bool DisplayData(std::ostream& out) const noexcept;
    /// @brief  Writes the data of the Entry if it is loaded to the path at GetPath(), fails if otherwise. 
    [[nodiscard]] bool WriteData() const noexcept;

    /// @brief  Reads from a specified input stream, only looking for the data. 
    [[nodiscard]] EmptyResult<std::string> Load(std::istream& in) noexcept;
    /// @brief Reads from the path located at GetPath(), only looking for the data. If the path could not be resolves, it attempts to create it. If it fails to create, it will return false, otherwise, data will be nullptr. 
    [[nodiscard]] EmptyResult<std::string> Load() noexcept;
    /// @brief Removes the 'Data' item from memory without deleting the file.
    void Unload() noexcept;
    /// @brief Deletes 'Data' from memory & the file system.
    void Reset() noexcept;

    [[nodiscard]] const VariableType& Data() const;
    void Data(std::unique_ptr<VariableType>&& New) noexcept;

    [[nodiscard]] std::optional<bool> HasData() const noexcept;
    [[nodiscard]] bool IsModified() const noexcept;

    void LoadImmediate(bool New) noexcept;
    void IsReadOnly(bool New) noexcept;
    
    [[nodiscard]] std::filesystem::path GetPath() const;
    [[nodiscard]] const PackageEntryIndex& GetIndex() const noexcept;
};

std::ostream& operator<<(std::ostream& out, const PackageEntry& obj) noexcept;


#endif //JASON_PACKAGEENTRY_H
