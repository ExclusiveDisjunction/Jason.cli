//
// Created by exdisj on 8/7/24.
//

#include <algorithm>

#include "Package.h" 
#include "PackageEntryIndex.h"
#include "../Common.h"

Package::Package(std::filesystem::path location, unsigned long ID, PackageHeader&& header, PackageIndex&& index, bool isCompressed) : location(location), packID(ID), header(std::move(header)), index(std::move(index))
{
    state |= (isCompressed ? Compressed : None);
}
Package::~Package()
{
    Close();
}

bool Package::IndexEntries() 
{
    if (!this->index.ReadIndex(this->entries))
        return false;

    for (PackageEntry* entry : this->entries)
    {
        if (entry->GetIndex().LoadImmediate() && !entry->Load())
            throw std::logic_error("For entry = " + this->name + "::" + entry->GetIndex().Name() + ", the data could not be loaded and the Load Idemedatley flag was set");
    }

    return true;
}

std::vector<PackageEntry*>::const_iterator Package::GetEntry(unsigned long ID) const noexcept
{
    return std::find_if(this->entries.begin(), this->entries.end(), [ID](const PackageEntry* item) -> bool
    {
        return item->Key().EntryID == ID;
    });
}
std::vector<PackageEntry*>::iterator Package::GetEntry(unsigned long ID) noexcept
{
    return std::find_if(this->entries.begin(), this->entries.end(), [ID](PackageEntry* item) -> bool
    {
        return item->Key().EntryID == ID;
    });
}

Package* Package::OpenFromDirectory(std::filesystem::path& dir, unsigned long ID)
{
    return nullptr;
}
Package* Package::OpenFromCompressed(std::filesystem::path& pack, std::filesystem::path& targetDir, unsigned long ID)
{
    return OpenFromDirectory(targetDir, ID);
}

const std::filesystem::path& Package::Location() const noexcept
{
    return this->location;
}
std::filesystem::path Package::VarLocation() const noexcept 
{
    return this->location / "var";
}
unsigned long Package::GetID() const noexcept 
{
    return this->packID;
}
const std::string& Package::GetName() const noexcept 
{
    return this->name;
}
bool Package::IsCompressed() const noexcept 
{
    return this->state & Compressed;
}
const PackageHeader& Package::Header() const noexcept 
{
    return this->header;
}
PackageHeader& Package::Header() noexcept 
{
    return const_cast<PackageHeader&>(const_cast<const Package*>(this)->Header());
}

bool Package::Compress(std::ostream& out) const noexcept
{
    if (!out)
        return false;

    out << "jason\n";
    out << this->header << '\n';
    out << "index\n";
    if (!this->index.Write(out, this->entries))
        return false;
    out << "\ndata\n";

    for (const auto* entry : this->entries)
        out << entry->Key().EntryID << ' ' << *entry << '\n';

    return out.good();
}
void Package::Close() noexcept
{
    index.Close();
    header.Close();

    for (auto& item : entries)
    {
        item->parent = nullptr;
        delete item;
        item = nullptr;
    }
    entries.clear();
}

const PackageEntry& Package::ResolveEntry(const std::string& name) const
{
    auto iter = std::find_if(this->entries.begin(), this->entries.end(), [&name](const PackageEntry* obj) -> bool 
    {
        return obj->GetIndex().Name() == name;
    });

    if (iter == this->entries.end())
        throw std::logic_error("Cannot find an entry with that name.");
    else
        return *(*iter);
}
PackageEntry& Package::ResolveEntry(const std::string& name)
{
    return const_cast<PackageEntry&>(const_cast<const Package*>(this)->ResolveEntry(name));
}
const PackageEntry& Package::ResolveEntry(PackageEntryKey key) const
{
    auto target = GetEntry(key.EntryID);
    if (target == this->entries.end() || key.PackageID != this->packID)
        throw std::logic_error("This entry is not in this package");

    return *(*target);
}
PackageEntry& Package::ResolveEntry(PackageEntryKey key)
{
    return const_cast<PackageEntry&>(const_cast<const Package*>(this)->ResolveEntry(key));
}

bool Package::RemoveEntry(unsigned long ID) noexcept
{
    PackageEntry* released = this->ReleaseEntry(ID);
    delete released;
    return released != nullptr;
}
PackageEntry* Package::ReleaseEntry(unsigned long ID) noexcept
{  
    auto iter = this->GetEntry(ID);

    if (iter == this->entries.end())
        return nullptr;
    
    PackageEntry* target = *iter;
    this->entries.erase(iter);

    return target;
}
std::optional<PackageEntryKey> Package::AddEntry(std::string name, PackageEntryType type, VariableType* data) noexcept
{
    if (type != PackageEntryType::Temporary && name.empty())
        return {};

    PackageEntry* result = new PackageEntry(PackageEntryKey(this->packID, this->GetNextID()), data, PackageEntryIndex(type, name, 0), this);
    this->entries.push_back(result);
    return result->Key();
}

bool Package::DoesEntryExist(unsigned long ID) noexcept
{
    return GetEntry(ID) != this->entries.end();
}