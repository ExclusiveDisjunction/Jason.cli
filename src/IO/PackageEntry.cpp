//
// Created by exdisj on 8/7/24.
//

#include "PackageEntry.h"
#include "Package.h"
#include "../Core/Common.h"

#include "PackageIndex.h"

#include <utility>
#include <string>
#include <fstream>
#include <sstream>

PackageEntry::PackageEntry(PackageEntryIndex&& index, std::weak_ptr<PackageReference> parent) : index(std::move(index)), parent(std::move(parent)), data()
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

void PackageEntry::Load()
{
    if (this->data)
        return;

    std::filesystem::path thisPath = this->GetPath();
    std::ifstream in(thisPath);
    if (!in) //Could not load at path
    {
        in.close();
        std::ofstream out(thisPath, std::ios::trunc);
        if (!out) //Failed to construct the path
        {
            std::stringstream errStr;
            errStr << "When loading package entry '";
            if (auto par = this->parent.lock()) 
                errStr << par->Target.GetName() << "::";
            
            errStr << this->index.name << "', the located path could not be found";
            throw std::logic_error(errStr.str());
        }  
        
        out << "NULL";
        out.close();
        this->data = nullptr;
    }
    else
        Load(in);
}
void PackageEntry::Load(std::istream& in)
{
    if (!in)
        throw std::logic_error("The file handle provided is invalid");

    in.seekg(0, std::ios::beg);
    this->data = std::move(VariableType::Desterilize(in));
}
bool PackageEntry::LoadNoThrow(std::string& message) noexcept
{ 
    try 
    {
        Load();
        return true;
    }
    catch (const std::logic_error& e)
    {
        message = e.what();
        return false;
    }
}
void PackageEntry::Unload() noexcept
{
    if (this->data.has_value())
        this->data.value().reset();

    this->data = {};
}
void PackageEntry::Reset() noexcept
{
    if (!parent.expired())
    {
        std::filesystem::path thisPath = this->GetPath();
        std::ofstream out(thisPath, std::ios::trunc);
    }

    Unload();
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
    if (parent.expired())
        throw std::logic_error("Could not locate parent's path.");

    auto par = parent.lock();
    return par->Target.VarLocation() / std::to_string(this->index.Key().EntryID);
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