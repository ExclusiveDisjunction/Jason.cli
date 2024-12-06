//
// Created by exdisj on 8/7/24.
//

#include "PackageEntry.h"
#include "Package.h"
#include "PackagePager.h"
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

bool PackageEntry::WriteData(PackagePager& pager) noexcept
{
    if (!pager.Bind(this->index.key))
        return false;

    bool result = false;
    if (this->data)
    {
        VariableType* obj = this->data->get();
        unsigned needed = obj->RequiredUnits();
        if (!pager.EnsureAllocation(needed)) 
            return false;

        if (obj)
        {
            pager.MoveRelative(0);
            result = pager.WriteUnits(obj->ToBinary());
        }
        else 
            result = pager.WipeAll();
    }

    pager.Reset();
    return result;
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
bool PackageEntry::WriteData() noexcept
{
    if (!IsModified() || !this->IsLoaded())
        return true;

    if (auto package = parent.lock())
    {
        PackagePager& pager = package->Target.Pager();
        if (!WriteData(pager))
            return false;
        
        this->modified = false;
        
        return true;
    }
    else 
        return false;
}

void PackageEntry::Load()
{
    if (this->data)
        return;

    if (auto par = this->parent.lock())
    {
        Package& host = par->Target;
        Load(host.Pager());
    }
    else
        throw std::logic_error("The parent package could not be resolved for loading");
}
void PackageEntry::Load(PackagePager& in)
{
    in.Bind(this->index.key);

    std::vector<Unit> allUnits = in.ReadAllUnits();
    if (allUnits.empty() || this->index.data_type == VT_None)
        this->data = nullptr;
    else 
        this->data = VariableType::FromBinary(allUnits, this->index.data_type);
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
bool PackageEntry::Unload() noexcept
{
    if (!this->WriteData())
        return false;

    if (this->data.has_value())
        this->data.value().reset();

    this->data = {};
    return true;
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
void PackageEntry::Data(std::unique_ptr<VariableType>&& New) noexcept
{
    Unload();

    this->data = std::move(New);
    this->index.data_type = !this->data ? VT_None : (*this->data)->GetType();
    this->modified = true;
}

bool PackageEntry::IsLoaded() const noexcept
{
    return this->data.has_value();
}
std::optional<bool> PackageEntry::HasData() const noexcept
{
    return !this->data ? std::optional<bool>() : *this->data != nullptr;
}
bool PackageEntry::IsModified() const noexcept
{
    return this->modified;
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