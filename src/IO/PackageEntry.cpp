//
// Created by exdisj on 8/7/24.
//

#include "PackageEntry.h"
#include "../Common.h"

#include <utility>

PackageEntry::PackageEntry(PackageEntryKey key, std::string name, VariableType* data, PackageEntryType type, unsigned char state) : key(key), name(std::move(name)), data(data), type(type), state(state)
{
    if (this->type != PackageEntryType::Temporary && name.empty())
        throw std::logic_error("Cannot construct a variable entry with no name, unless type is temporary.");

    if (this->type == PackageEntryType::Temporary)
        this->name.clear();
}
PackageEntry::PackageEntry(PackageEntry&& obj) noexcept : key(obj.key), type(obj.type), name(std::move(obj.name)), data(std::exchange(obj.data, nullptr)), state(obj.state)
{

}
PackageEntry::~PackageEntry()
{
    delete data;
    data = nullptr;
}

[[nodiscard]] bool PackageEntry::WriteCompressedLine(std::ostream& out) const noexcept
{
    return WriteSchematic(out) && (out << ' ') && WriteData(out);
}
[[nodiscard]] bool PackageEntry::WriteData(std::ostream& out) const noexcept
{
    if (!out)
        return false;

    if (this->data)
        this->data->Sterilize(out);
    else
        out << "NULL";

    return out.good();
}
[[nodiscard]] bool PackageEntry::WriteSchematic(std::ostream& out) const noexcept
{
    if (!out)
        return false;

    out << this->key.EntryID << ' ' << (this->type == Variable ? "var" : this->type == Environment ? "env" : "tmp") << " f:" << (this->state & load_imm ? '!' : 0) << (this->state & readonly && !(this->state & readonly_package) ? '~' : 0) << ' ' << this->name;
    return true;
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
[[nodiscard]] bool PackageEntry::LoadImm() const noexcept
{
    return this->load_imm;
}
[[nodiscard]] PackageEntryType PackageEntry::GetType() const noexcept
{
    return this->type;
}
[[nodiscard]] bool PackageEntry::IsTemporary() const noexcept
{
    return this->type == Temporary;
}
[[nodiscard]] std::filesystem::path PackageEntry::GetPath(const std::filesystem::path& source) const noexcept
{
    return source / std::to_string(this->key.EntryID);
}




