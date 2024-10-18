//
// Created by exdisj on 8/7/24.
//

#include "PackageEntry.h"
#include "Package.h"
#include "../Common.h"

#include "PackageIndex.h"

#include <utility>
#include <string>
#include <fstream>
#include <sstream>

PackageEntry::PackageEntry(PackageEntryIndex&& index, Package* parent) : index(std::move(index)), parent(parent), data()
{
    if (this->index.type != PackageEntryType::Temporary && this->index.name.empty())
        throw std::logic_error("Cannot construct a variable entry with no name, unless type is temporary.");
    
    if (this->index.type == PackageEntryType::Temporary)
        this->index.name.clear();
}
PackageEntry::~PackageEntry()
{
    (void)Unload();
}

bool PackageEntry::WriteData(std::ostream& out) const noexcept
{
    if (!out || !this->data)
        return false;

    if (*this->data)
        (*this->data)->Sterilize(out);
    else
        out << "NULL";

    out.flush();
    return !out.bad();
}
bool PackageEntry::DisplayData(std::ostream& out) const noexcept
{
    if (!this->data.has_value())
        out << "(Unloaded)";
    else if (!(*this->data))
        out << "NULL";
    else
        (*this->data)->Print(out);

    return !out.bad();
}
bool PackageEntry::WriteData() const noexcept
{
    std::filesystem::path thisPath = this->GetPath();
    std::ofstream out(thisPath, std::ios::trunc);

    return WriteData(out);
}

bool PackageEntry::Load() noexcept
{
    if (this->data)
        return true;

    std::filesystem::path thisPath = this->GetPath();
    std::ifstream in(thisPath);
    if (!in) //Could not load at path
    {
        in.close();
        std::ofstream out(thisPath, std::ios::trunc);
        if (!out) //Failed to construct the path
        {
            std::cerr << "When loading package entry '" << (this->parent ? this->parent->GetName() : "") << "::" << this->index.name << "', the located path could not be found." << std::endl;
            return false;
        }  
        
        out << "NULL";
        out.close();
        this->data = nullptr;
        return true;
    }
    else
        return Load(in);
}
bool PackageEntry::Load(std::istream& in) noexcept 
{
    if (!in)
        return false;

    in.seekg(0, std::ios::beg);
    this->data = VariableType::Desterilize(in);
    return this->data != nullptr;
}
void PackageEntry::Unload() noexcept
{
    if (this->data.has_value())
        this->data.value().reset();

    this->data = {};
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

    Unload();
    return true;
}

const VariableType& PackageEntry::Data() const
{
    if (!this->data.has_value())
        throw std::logic_error("The data contained is not loaded.");
    else if (! *(this->data))
        throw std::logic_error("This package entry contains no data.");

    return *(*this->data);
}
bool PackageEntry::Data(std::unique_ptr<VariableType>&& New) noexcept
{
    Unload();

    this->data = std::move(New);
    return WriteData();
}

std::optional<bool> PackageEntry::HasData() const noexcept
{
    return !this->data ? std::optional<bool>() : *this->data != nullptr;
}
bool PackageEntry::IsModified() const noexcept
{
    return this->modified || this->index.IsModified();
}

void PackageEntry::LoadImmediate(bool New) noexcept 
{
    this->index.LoadImmediate(New);
}
void PackageEntry::IsReadOnly(bool New) noexcept 
{
    this->index.IsReadOnly(New);
}

std::filesystem::path PackageEntry::GetPath() const
{
    if (!this->parent)
        throw std::logic_error("Could not locate parent's path.");

    return this->parent->VarLocation() / std::to_string(this->index.Key().EntryID);
}
const PackageEntryIndex& PackageEntry::GetIndex() const noexcept
{
    return this->index;
}

std::ostream& operator<<(std::ostream& out, const PackageEntry& obj) noexcept
{
    (void)obj.WriteData(out);
    return out;
}