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
        readonly = 2,
        readonly_package = 4 //When true, the top package that this comes from is read-only opened.
    };

    PackageEntryKey key;
    PackageEntryType type;
    std::string name;
    VariableType* data;
    unsigned char state;
    std::optional<std::pair<std::streampos, std::streampos>> loc;

    PackageEntry(PackageEntryKey key, std::string name, VariableType* data, PackageEntryType type, unsigned char state);

public:
    PackageEntry(const PackageEntry& obj) = delete;
    PackageEntry(PackageEntry&& obj) noexcept;
    ~PackageEntry();

    [[nodiscard]] PackageEntry* FromIndexTableLine(std::istream& in, unsigned long PackageID);
    [[nodiscard]] PackageEntry* FromIndexTableLine(const std::string& line, unsigned long PackageID);
    [[nodiscard]] PackageEntry* FromCompressedLineRO(std::istream& in, PackageEntryKey key);
    [[nodiscard]] PackageEntry* FromCompressedLine(std::istream& in, std::ostream& out, PackageEntryKey key);

    PackageEntry& operator=(const PackageEntry& obj) = delete;
    PackageEntry& operator=(PackageEntry&& obj) noexcept = delete;

    [[nodiscard]] bool WriteCompressedLine(std::ostream& out) const noexcept;
    [[nodiscard]] bool WriteSchematic(std::ostream& out) const noexcept;
    [[nodiscard]] bool WriteData(std::ostream& out) const noexcept;

    [[nodiscard]] const VariableType& Data() const;
    void Data(VariableType* New) noexcept;

    [[nodiscard]] bool HasData() const noexcept;
    [[nodiscard]] PackageEntryKey Key() const noexcept;
    [[nodiscard]] const std::string& Name() const noexcept;
    [[nodiscard]] bool LoadImm() const noexcept;
    [[nodiscard]] PackageEntryType GetType() const noexcept;
    [[nodiscard]] bool IsTemporary() const noexcept;
    [[nodiscard]] std::filesystem::path GetPath(const std::filesystem::path& source) const noexcept;
};


#endif //JASON_PACKAGEENTRY_H
