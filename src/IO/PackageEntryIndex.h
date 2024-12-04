#ifndef JASON_PACKAGEENTRYINDEX_H
#define JASON_PACKAGEENTRYINDEX_H

#include <iostream>
#include <fstream>
#include <string>

#include "PackageEntryKey.h"
#include "../Calc/VariableType.h"

enum PackageEntryType
{
    Variable,
    Temporary,
    Environment
};

class PackageEntryIndex 
{
private:
    enum State
    {
        none = 0,
        load_imm = 1,
        readonly = 2
    };

    PackageEntryKey key;
    PackageEntryType type = PackageEntryType::Variable;
    VariableTypes data_type = VariableTypes::VT_None;
    std::string name;
    unsigned char state = 0;
    
public:
    PackageEntryIndex() = default;
    PackageEntryIndex(PackageEntryKey key, PackageEntryType type, std::string name, unsigned char state, VariableTypes data_type) noexcept;
    explicit PackageEntryIndex(std::istream& in);
    PackageEntryIndex(const PackageEntryIndex& obj) noexcept = default;
    PackageEntryIndex(PackageEntryIndex&& obj) noexcept = default;

    friend class PackageIndex;
    friend class PackageEntry;
    friend std::ostream& operator<<(std::ostream&, const PackageEntryIndex&) noexcept;
    friend std::istream& operator>>(std::istream&, PackageEntryIndex&);

    PackageEntryIndex& operator=(const PackageEntryIndex& obj) noexcept = default;
    PackageEntryIndex& operator=(PackageEntryIndex&& obj) noexcept = default;

    [[nodiscard]] bool LoadImmediate() const noexcept;
    [[nodiscard]] bool IsReadOnly() const noexcept;
    [[nodiscard]] bool IsTemporary() const noexcept;
    [[nodiscard]] const std::string& Name() const noexcept;
    [[nodiscard]] const PackageEntryKey& Key() const noexcept;

    void LoadImmediate(bool New) noexcept;
    void IsReadOnly(bool New) noexcept;
    void Name(const std::string& New) noexcept;
};

std::ostream& operator<<(std::ostream& out, const PackageEntryIndex& obj) noexcept;
std::istream& operator>>(std::istream& in, PackageEntryIndex& obj);

#endif