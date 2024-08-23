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
    VariableType* data;
    Package* parent;
    unsigned char state;

    PackageEntry(PackageEntryKey key, std::string name, VariableType* data, PackageEntryType type, Package* parent, unsigned char state = 0);

public:
    PackageEntry(const PackageEntry& obj) = delete;
    PackageEntry(PackageEntry&& obj) noexcept = delete;
    ~PackageEntry();

    friend class Package;

    [[nodiscard]] PackageEntry* FromIndexTableLine(std::istream& in, Package* parent);
    [[nodiscard]] PackageEntry* FromIndexTableLine(const std::string& line, Package* parent);

    PackageEntry& operator=(const PackageEntry& obj) = delete;
    PackageEntry& operator=(PackageEntry&& obj) noexcept = delete;

    [[nodiscard]] bool WriteCompressedLine(std::ostream& out) const noexcept;
    [[nodiscard]] bool WriteSchematic(std::ostream& out) const noexcept;
    [[nodiscard]] bool WriteData(std::ostream& out) const noexcept;
    [[nodiscard]] bool ReadFromFile(std::istream& in) noexcept;

    [[nodiscard]] bool Load() noexcept;
    [[nodiscard]] bool Unload() noexcept;
    void Reset() noexcept;

    [[nodiscard]] const VariableType& Data() const;
    void Data(VariableType* New) noexcept;

    [[nodiscard]] bool HasData() const noexcept;
    [[nodiscard]] bool LoadIdmediatley() const noexcept;
    [[nodiscard]] bool IsReadOnly() const noexcept;
    [[nodiscard]] bool IsTemporary() const noexcept;

    [[nodiscard]] PackageEntryKey Key() const noexcept;
    [[nodiscard]] PackageEntryType Type() const noexcept;
    [[nodiscard]] std::filesystem::path GetPath(const std::filesystem::path& hostDir) const noexcept;
    [[nodiscard]] const std::string& Name() const noexcept;


};


#endif //JASON_PACKAGEENTRY_H
