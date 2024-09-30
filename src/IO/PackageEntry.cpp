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

PackageEntry::PackageEntry(std::optional<VariableType*> data, PackageEntryIndex&& index, Package* parent) : index(std::move(index)), parent(parent) 
{
    if (this->index.type != PackageEntryType::Temporary && this->index.name.empty())
        throw std::logic_error("Cannot construct a variable entry with no name, unless type is temporary.");
    
    if (this->index.type == PackageEntryType::Temporary)
        this->index.name.clear();

    if (data.has_value())
        this->Data(*data);
    else
        this->data = {};
}
PackageEntry::~PackageEntry()
{
    if (this->parent)
        (void)this->parent->ReleaseEntry(this->index.Key().EntryID);

    (void)Unload();
}

/*
PackageEntry* PackageEntry::FromIndexTableLine(std::istream& in, Package* parent)
{
    if (!in)
        return nullptr;

    PackageEntry* result = new PackageEntry();
    
    result->parent = parent;
    result->key.PackageID = !parent ? 0 : parent->GetID();
    ReadIndex(in, *result);

    return result;
}
PackageEntry* PackageEntry::FromIndexTableLine(const std::string& line, Package* parent)
{
    std::stringstream ss(line);
    return FromIndexTableLine(ss, parent);
}
PackageEntry* PackageEntry::ExpandFromCompressed(std::istream& in, Package* parent, std::ostream& out)
{
    PackageEntry result;
    if (!in)
        return nullptr;

    //Process header & fill structure
    result.parent = parent;
    result.key.PackageID = !parent ? 0 : parent->GetID();
    ReadIndex(in, result);

    //Output to the specified output location.
    std::string sterilized;
    std::getline(in, sterilized);
    
    std::stringstream ss_steril(sterilized);
    sterilized.clear();

    out << ss_steril.rdbuf();
    ss_steril.seekp(0, std::ios::beg);

    //Load from sterilized string, prevents opening & closing of file streams.
    if (result.state & load_imm)
    {
        if (!result.Load(ss_steril))
            throw std::logic_error("The load idmediatley flag was set, but the file could not be resolved, or it does not contain proper formatted data");
    }
}
PackageEntry* PackageEntry::ExpandFromCompressed(const std::string& line, Package* parent, std::ostream& out)
{
    std::stringstream ss(line);
    return ExpandFromCompressed(ss, parent, out);
}
*/

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
    this->data = VariableType::FromSterilized(in);
    return this->data != nullptr;
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
    (void)WriteData();
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