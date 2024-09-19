//
// Created by exdisj on 8/7/24.
//

#include <algorithm>

#include "Package.h"
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

}

const PackageEntry* Package::GetEntry(unsigned long ID) const noexcept 
{
    auto iter = std::find_if(this->entries.begin(), this->entries.end(), [ID](PackageEntry* item) -> bool
    {
        return item->Key().EntryID == ID;
    });

    if (iter == this->entries.end())
        return nullptr;
    else
        return *iter;
}
PackageEntry* Package::GetEntry(unsigned long ID) noexcept 
{
    return const_cast<PackageEntry*>(const_cast<const Package*>(this)->GetEntry(ID));
}

Package* Package::OpenFromDirectory(std::filesystem::path& dir, unsigned long ID)
{

}
Package* Package::OpenFromCompressed(std::filesystem::path& pack, std::filesystem::path& targetDir, unsigned long ID)
{

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