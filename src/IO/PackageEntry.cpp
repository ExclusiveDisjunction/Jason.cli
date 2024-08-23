//
// Created by exdisj on 8/7/24.
//

#include "PackageEntry.h"
#include "Package.h"
#include "../Common.h"

#include <utility>
#include <string>

PackageEntry::PackageEntry(PackageEntryKey key, std::string name, VariableType* data, PackageEntryType type, Package* parent, unsigned char state) : key(key), name(std::move(name)), data(nullptr), type(type), parent(parent), state(state)
{
    if (this->type != PackageEntryType::Temporary && name.empty())
        throw std::logic_error("Cannot construct a variable entry with no name, unless type is temporary.");

    if (this->type == PackageEntryType::Temporary)
        this->name.clear();

    this->Data(data);
}
PackageEntry::~PackageEntry()
{
    if (this->parent)
        this->parent->ReleaseEntry(this->key.EntryID);

    delete data;
    data = nullptr;
}

bool PackageEntry::WriteCompressedLine(std::ostream& out) const noexcept
{
    return WriteSchematic(out) && (out << ' ') && WriteData(out);
}
bool PackageEntry::WriteData(std::ostream& out) const noexcept
{
    if (!out)
        return false;

    if (this->data)
        this->data->Sterilize(out);
    else
        out << "NULL";

    return out.good();
}
bool PackageEntry::WriteSchematic(std::ostream& out) const noexcept
{
    if (!out)
        return false;

    out << this->key.EntryID << ' ' << (this->type == Variable ? "var" : this->type == Environment ? "env" : "tmp") << " f:" << (this->state & load_imm ? '!' : 0) << (this->state & readonly ? '~' : 0) << ' ' << this->name;
    return true;
}
bool PackageEntry::ReadFromFile(std::istream& target) noexcept
{

    if (this->loc.has_value())
        target.seekg((*this->loc).first);
    else
        target.seekg(0, std::ios::beg);

    if (!target)
        return false;

    std::string type;
    auto loc = target.tellg();
    target >> type;

    this->Data(nullptr);

    if (type.empty())
        return false;
    else if (type == "NULL") //empty, we just return true since data == nullptr.
        return true;
    else //We try to construct it.
    {
        target.seekg(loc);
        VariableType* temp = VariableType::FromSterilized(target);   
        if (!temp)
            return false;
        
        this->Data(temp);
        temp = nullptr;
        return true;
    }
}

const VariableType& PackageEntry::Data() const
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

bool PackageEntry::HasData() const noexcept
{
    return data != nullptr;
}
PackageEntryKey PackageEntry::Key() const noexcept
{
    return state & load_imm;
}
const std::string& PackageEntry::Name() const noexcept
{
    return state & readonly;
}
bool PackageEntry::LoadImm() const noexcept
{
    return type == Temporary;
}
PackageEntryType PackageEntry::GetType() const noexcept
{
    return this->type;
}
bool PackageEntry::IsTemporary() const noexcept
{
    return type;
}
std::filesystem::path PackageEntry::GetPath(const std::filesystem::path& source) const noexcept
{
    return source / std::to_string(this->key.EntryID);
}