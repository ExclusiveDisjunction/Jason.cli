//
// Created by exdisj on 8/7/24.
//

#include "PackageEntry.h"
#include "Package.h"
#include "../Common.h"

#include <utility>
#include <string>
#include <fstream>

PackageEntry::PackageEntry(PackageEntryKey key, std::string name, VariableType* data, PackageEntryType type, Package* parent, unsigned char state) : key(key), name(std::move(name)), data(nullptr), type(type), parent(parent), state(state)
{
    if (this->type != PackageEntryType::Temporary && name.empty())
        throw std::logic_error("Cannot construct a variable entry with no name, unless type is temporary.");

    if (this->type == PackageEntryType::Temporary)
        this->name.clear();

    this->Data(data);
}
PackageEntry::PackageEntry(PackageEntry&& obj) noexcept : key(std::exchange(obj.key, PackageEntryKey())), 
    type(obj.type), 
    name(std::move(obj.name)), 
    data(std::exchange(data, std::optional<VariableType*>()), 
    parent(std::exchange(parent, nullptr)), 
    state(std::exchange(obj.state, static_cast<unsigned char>(0)))
{
    if (this->parent)
        this->parent->SwapEntry(this->key.EntryID, this);
}
PackageEntry::~PackageEntry()
{
    if (this->parent)
        this->parent->ReleaseEntry(this->key.EntryID);

    (void)Unload();
}

PackageEntry* PackageEntry::FromIndexTableLine(std::istream& in, Package* parent)
{

}
PackageEntry* PackageEntry::FromIndexTableLine(const std::string& line, Package* parent)
{

}
PackageEntry* PackageEntry::ExpandFromCompressed(std::istream& in, Package* parent, std::ostream& out)
{

}
PackageEntry* PackageEntry::ExpandFromCompressed(const std::string& line, Package* parent, std::ostream& out)
{

}

bool PackageEntry::WriteCompressedLine(std::ostream& out) const noexcept
{
    return WriteSchematic(out) && (out << ' ') && WriteData(out);
}
bool PackageEntry::WriteData(std::ostream& out) const noexcept
{
    if (!out || !this->data)
        return false;

    if (*this->data)
        (*this->data)->Sterilize(out);
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
/*
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
*/

bool PackageEntry::Load() noexcept
{
    if (this->data)
        return true;

    std::filesystem::path thisPath = this->GetPath();
    std::ifstream in(thisPath);
    if (!in)
        return false;

    in.seekg(0, std::ios::beg);
    this->data = VariableType::FromSterilized(in);
}
bool PackageEntry::Unload() noexcept
{
    delete this->data.value_or(nullptr);
    this->data = {};
    return true;
}
bool PackageEntry::Reset() noexcept
{
    if (this->parent)
    {
        std::filesystem::path thisPath = this->GetPath();
        std::ofstream out(thisPath, std::ios::trunc);
        if (!out)
            return false;
    }

    return Unload();
}

const VariableType& PackageEntry::Data() const
{
    if (!this->data.has_value())
        throw std::logic_error("The data contained is not loaded.");
    else if (! *(this->data))
        throw std::logic_error("This package entry contains no data.");

    return *(*this->data);
}
void PackageEntry::Data(VariableType* New) noexcept
{
    Reset();
    this->data = New;
}

std::optional<bool> PackageEntry::HasData() const noexcept
{
    return !this->data ? std::optional<bool>() : *this->data != nullptr;
}

bool PackageEntry::LoadImmediate() const noexcept
{
    return this->state & load_imm;
}
bool PackageEntry::IsReadOnly() const noexcept
{
    return this->state & readonly;
}
bool PackageEntry::IsTemporary() const noexcept
{
    return this->type == Temporary;
}

PackageEntryKey PackageEntry::Key() const noexcept
{
    return this->key;
}
PackageEntryType PackageEntry::Type() const noexcept
{
    return this->type;
}
std::filesystem::path PackageEntry::GetPath() const
{
    if (!this->parent)
        throw std::logic_error("Could not locate parent's path.");

    return this->parent->VarLocation() / std::to_string(this->key.EntryID);
}
const std::string& PackageEntry::Name() const noexcept
{
    return this->name;
}