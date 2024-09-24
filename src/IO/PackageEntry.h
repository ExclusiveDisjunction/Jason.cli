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

class Package;

class PackageEntry
{
private:
    std::optional<VariableType*> data = {};
    Package* parent = nullptr;
    PackageEntryIndex index;
    bool modified = false;

    PackageEntry() : data(), parent(nullptr), index(), modified(false) {}

public:
    PackageEntry(VariableType* data, PackageEntryIndex&& schema, Package* parent);
    PackageEntry(const PackageEntry& obj) = delete;
    ~PackageEntry();

    friend class Package;

    /*
    [[nodiscard]] static PackageEntry* FromIndexTableLine(std::istream& in, Package* parent);
    [[nodiscard]] static PackageEntry* FromIndexTableLine(const std::string& line, Package* parent);
    [[nodiscard]] static PackageEntry* ExpandFromCompressed(std::istream& in, Package* parent, std::ostream& out);
    [[nodiscard]] static PackageEntry* ExpandFromCompressed(const std::string& line, Package* parent, std::ostream& out);
    */

    PackageEntry& operator=(const PackageEntry& obj) = delete;
    PackageEntry& operator=(PackageEntry&& obj) noexcept = delete;

    /// @breif Writes the header schematic of the Entry
    [[nodiscard]] bool WriteIndex(std::ostream& out) const noexcept;
    /// @breif Writes the data of the Entry if it is loaded, fails if otherwise.
    [[nodiscard]] bool WriteData(std::ostream& out) const noexcept;
    /// @brief  Writes the data of the Entry if it is loaded to the path at GetPath(), fails if otherwise. 
    [[nodiscard]] bool WriteData() const noexcept;

    /// @brief  Reads from a specified input stream, only looking for the data. 
    [[nodiscard]] bool Load(std::istream& in) noexcept;
    /// @brief Reads from the path located at GetPath(), only looking for the data. If the path could not be resolves, it attempts to create it. If it fails to create, it will return false, otherwise, data will be nullptr. 
    [[nodiscard]] bool Load() noexcept;
    /// @brief Removes the 'Data' item from memory without deleting the file.
    [[nodiscard]] bool Unload() noexcept;
    /// @brief Deletes 'Data' from memory & the file system.
    bool Reset() noexcept;

    [[nodiscard]] const VariableType& Data() const;
    void Data(VariableType* New) noexcept;

    [[nodiscard]] std::optional<bool> HasData() const noexcept;
    [[nodiscard]] bool IsModified() const noexcept;

    void LoadImmediate(bool New) noexcept;
    void IsReadOnly(bool New) noexcept;

    [[nodiscard]] PackageEntryKey Key() const noexcept;
    [[nodiscard]] std::filesystem::path GetPath() const;
    [[nodiscard]] const PackageEntryIndex& GetIndex() const noexcept;
};

std::ostream& operator<<(std::ostream& out, const PackageEntry& obj) noexcept;


#endif //JASON_PACKAGEENTRY_H
