//
// Created by exdisj on 8/7/24.
//

#ifndef JASON_PACKAGEENTRY_H
#define JASON_PACKAGEENTRY_H

#include <string>
#include <filesystem>
#include <optional>

#include "PackageEntryKey.h"
#include "../Calc/VariableType.h"

class Package;

enum PackageEntryType
{
    Variable,
    Temporary,
    Environment
};

class PackageEntry
{
private:
    enum State
    {
        none = 0,
        load_imm = 1,
        readonly = 2
    };

    PackageEntryKey key;
    PackageEntryType type;
    std::string name;
    std::optional<VariableType*> data = {};
    Package* parent = nullptr;
    unsigned char state = 0;

    PackageEntry() : key(), name(), data(std::optional<VariableType*>()), type(PackageEntryType::Variable), parent(nullptr), state(0) {}
    PackageEntry(PackageEntryKey key, std::string name, VariableType* data, PackageEntryType type, Package* parent, unsigned char state = 0);

public:
    PackageEntry(const PackageEntry& obj) = delete;
    PackageEntry(PackageEntry&& obj) noexcept;
    ~PackageEntry();

    friend class Package;

    [[nodiscard]] static PackageEntry* FromIndexTableLine(std::istream& in, Package* parent);
    [[nodiscard]] static PackageEntry* FromIndexTableLine(const std::string& line, Package* parent);
    [[nodiscard]] static PackageEntry* ExpandFromCompressed(std::istream& in, Package* parent, std::ostream& out);
    [[nodiscard]] static PackageEntry* ExpandFromCompressed(const std::string& line, Package* parent, std::ostream& out);

    PackageEntry& operator=(const PackageEntry& obj) = delete;
    PackageEntry& operator=(PackageEntry&& obj) noexcept = delete;

    /// @breif Writes the information of the Entry into one line for the compressed Jason format.
    [[nodiscard]] bool WriteCompressedLine(std::ostream& out) const noexcept;
    /// @breif Writes the header schematic of the Entry
    [[nodiscard]] bool WriteSchematic(std::ostream& out) const noexcept;
    /// @breif Writes the data of the Entry if it is loaded, fails if otherwise.
    [[nodiscard]] bool WriteData(std::ostream& out) const noexcept;

    [[nodiscard]] bool Load() noexcept;
    [[nodiscard]] bool Unload() noexcept;
    bool Reset() noexcept;

    [[nodiscard]] const VariableType& Data() const;
    void Data(VariableType* New) noexcept;

    [[nodiscard]] std::optional<bool> HasData() const noexcept;

    [[nodiscard]] bool LoadImmediate() const noexcept;
    [[nodiscard]] bool IsReadOnly() const noexcept;
    [[nodiscard]] bool IsTemporary() const noexcept;

    [[nodiscard]] PackageEntryKey Key() const noexcept;
    [[nodiscard]] PackageEntryType Type() const noexcept;
    [[nodiscard]] std::filesystem::path GetPath() const;
    [[nodiscard]] const std::string& Name() const noexcept;


};


#endif //JASON_PACKAGEENTRY_H
