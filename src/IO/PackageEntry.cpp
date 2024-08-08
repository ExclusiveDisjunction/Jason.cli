//
// Created by exdisj on 8/7/24.
//

#include "PackageEntry.h"

#include <utility>

PackageEntry::PackageEntry(PackageEntryKey key, std::string name, VariableType* data, PackageEntryType type, bool load_imm) : key(key), name(std::move(name)), data(data), type(type), load_imm(load_imm)
{
    if (this->type != PackageEntryType::Temporary && name.empty())
        throw std::logic_error("Cannot construct a variable entry with no name, unless type is temporary.");

    if (this->type == PackageEntryType::Temporary)
        this->name.clear();
}
PackageEntry::~PackageEntry()
{
    delete data;
    data = nullptr;
}

[[nodiscard]] bool PackageEntry::Write(std::ostream& out) const noexcept
{
    if (!out || !this->data)
        return false;

    switch (this->type)
    {
        case PackageEntryType::Variable:
            out << "VAR";
            break;
        case PackageEntryType::Environment:
            out << "ENV";
            break;
        case PackageEntryType::Temporary:
        default:
            return false;
    }

    out << " ";
    if (this->load_imm)
        out << "! ";

    return WriteData(out);
}
[[nodiscard]] bool PackageEntry::WriteData(std::ostream& out) const noexcept
{
    if (!out || !this->data)
        return false;

    this->data->Sterilize(out);
    return out.good();
}

[[nodiscard]] const VariableType& PackageEntry::Data() const
{
    if (!this->data)
        throw std::logic_error("Entry is empty, cannot dereference.");

    return *this->data;
}
void PackageEntry::Data(VariableType* New) noexcept
{
    delete data;
    this->data = New;
}

[[nodiscard]] bool PackageEntry::HasData() const noexcept
{
    return this->data != nullptr;
}
[[nodiscard]] PackageEntryKey PackageEntry::Key() const noexcept
{
    return this->key;
}
[[nodiscard]] const std::string& PackageEntry::Name() const noexcept
{
    return this->name;
}




