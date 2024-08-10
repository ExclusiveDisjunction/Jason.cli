//
// Created by exdisj on 8/7/24.
//

#ifndef JASON_PACKAGEENTRY_H
#define JASON_PACKAGEENTRY_H

#include <string>
#include <filesystem>

#include "PackageEntryKey.h"
#include "../Calc/VariableType.h"

enum PackageEntryType
{
    Variable,
    Temporary,
    Environment
};

class PackageEntry
{
private:
    PackageEntryKey key;
    PackageEntryType type;
    std::string name;
    VariableType* data;
    bool load_imm;

public:
    PackageEntry(PackageEntryKey key, std::string name, VariableType* data, PackageEntryType type, bool load_imm = false);
    PackageEntry(const PackageEntry& obj) = delete;
    PackageEntry(PackageEntry&& obj) noexcept = delete;
    ~PackageEntry();

    PackageEntry& operator=(const PackageEntry& obj) = delete;
    PackageEntry& operator=(PackageEntry&& obj) noexcept = delete;

    [[nodiscard]] bool Write(std::ostream& out) const noexcept;
    [[nodiscard]] bool WriteData(std::ostream& out) const noexcept;
    [[nodiscard]] bool ReadFromFile(std::istream& in) noexcept;

    [[nodiscard]] const VariableType& Data() const;
    void Data(VariableType* New) noexcept;

    [[nodiscard]] bool HasData() const noexcept;
    [[nodiscard]] PackageEntryKey Key() const noexcept;
    [[nodiscard]] const std::string& Name() const noexcept;
    [[nodiscard]] bool LoadImm() const noexcept;
    [[nodiscard]] bool IsTemporary() const noexcept { return this->type == Temporary; }
    [[nodiscard]] std::filesystem::path GetPath(const std::filesystem::path& source) const noexcept;
};


#endif //JASON_PACKAGEENTRY_H
