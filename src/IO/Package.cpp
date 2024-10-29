//
// Created by exdisj on 8/7/24.
//

#include <algorithm>

#include "Package.h" 
#include "PackageEntryIndex.h"
#include "UnloadedPackage.h"
#include "../Core/Common.h"

using namespace std::filesystem;

Package::Package(unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index) : packID(ID), name(std::move(name)), header(std::move(header)), index(std::move(index)), state(0)
{

}
Package::Package(path location, unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index) : Package(ID, std::move(name), std::move(header), std::move(index))
{
    this->location = std::move(location);
    this->compressedLocation = {};
}
Package::Package(path uLocation, path cLocation, unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index) : Package(ID, std::move(name), std::move(header), std::move(index))
{
    this->location = std::move(uLocation);
    this->compressedLocation = std::move(cLocation);
    this->state |= Compressed;
}
Package::~Package()
{
    Close();
}

EmptyResult<std::string> Package::IndexEntries() noexcept
{
    std::vector<PackageEntryIndex> indexes = this->index.ReadIndex(this->packID);
    if (indexes.empty())
        return {};

    for (PackageEntryIndex& obj : indexes)
    {
        this->currID = std::max(this->currID, obj.Key().EntryID);

        PackageEntry& result = this->entries.emplace_back(std::move(obj), this);
        
        if (result.GetIndex().LoadImmediate() && !result.Load())
            return std::string("For entry = " + this->name + "::" + result.GetIndex().Name() + ", the data could not be loaded and the Load Idemedatley flag was set");
    }
    //We need to increment the currID, because right now it has the highest value of the current key.
    this->currID++;

    return EmptyResult<std::string>();
}

std::vector<PackageEntry>::const_iterator Package::GetEntry(unsigned long ID) const noexcept
{
    return std::find_if(this->entries.begin(), this->entries.end(), [ID](const PackageEntry& item) -> bool
    {
        return item.GetIndex().Key().EntryID == ID;
    });
}
std::vector<PackageEntry>::iterator Package::GetEntry(unsigned long ID) noexcept
{
    return std::find_if(this->entries.begin(), this->entries.end(), [ID](const PackageEntry& item) -> bool
    {
        return item.GetIndex().Key().EntryID == ID;
    });
}

Result<std::shared_ptr<Package>, std::string> Package::OpenFromDirectory(const std::filesystem::path& dir, unsigned long ID) noexcept
{
    /*
     * We look for the following things:
     * 1 -> Header file
     * 2 -> Index File
     * 3 -> var directory
     *
     * Then we load the header, then index.
     */

    std::filesystem::path header_l(dir / "header"), index_l(dir / "index"), var_l(dir / "var");
    Result<FileHandle, std::string> header_f = FileHandle::TryOpen(header_l), index_f = FileHandle::TryOpen(index_l);
    if (header_f.IsErr())
        return header_f.GetErrDirect();
    if (index_f.IsErr())
        return index_f.GetErrDirect();
    if (!std::filesystem::exists(var_l))
        return std::string("The variable folder does not exist");

    Result<PackageHeader, std::string> header = PackageHeader::OpenHeader(std::move(header_f.GetOkDirect()));
    PackageIndex index(std::move(index_f.GetOkDirect()));
    if (header.GetErr())
        return header.GetErrDirect();

    std::shared_ptr<Package> result( new Package(dir, ID, dir.filename(), std::move(header.GetOkDirect()), std::move(index)) );

    EmptyResult<std::string> indexStatus = result->IndexEntries();
    if (indexStatus.IsErr())
        return indexStatus.GetErrDirect();
    else
        return result;
}
Result<std::shared_ptr<Package>, std::string> Package::OpenFromCompressed(const std::filesystem::path& pack, const std::filesystem::path& targetDir, unsigned long ID) noexcept
{
    return std::string("Not implemented yet.");
}
Result<std::shared_ptr<Package>, std::string> Package::OpenFromUnloaded(const UnloadedPackage& obj) noexcept
{
    return OpenFromDirectory(obj.Target, obj.PackID);
}
Result<std::shared_ptr<Package>, std::string> Package::NewPackage(const std::string& name, const std::filesystem::path& landingDirectory, unsigned long ID) noexcept
{
    if (!std::filesystem::exists(landingDirectory) || !std::filesystem::is_directory(landingDirectory) || name.empty())
        return std::string("Landing directory is not a directory, does not exist, or the name is empty");

    std::filesystem::path path = landingDirectory / name;
    if (std::filesystem::exists(path)) //Already exists
        return OpenFromDirectory(std::move(path), ID);

    if (!std::filesystem::create_directory(path))
        return std::string("Unable to create directory");

    std::filesystem::path header_p = path / "header", index_p = path / "index", entries_p = path / "var";

    if (!std::filesystem::create_directory(entries_p))
        return std::string("Unable to create entries directory");

    Result<FileHandle, std::string> header_f = FileHandle::TryOpen(header_p), index_f = FileHandle::TryOpen(index_p);
    if (header_f.IsErr())
        return header_f.GetErrDirect();
    if (index_f.IsErr())
        return index_f.GetErrDirect();

    PackageHeader header(std::move(header_f.GetOkDirect()));
    PackageIndex index(std::move(index_f.GetOkDirect()));
    if (!header.Write())
        return std::string("The header could not be written to the file system"); 

    return std::shared_ptr<Package>( new Package(path, ID, std::move(name), std::move(header), std::move(index)) );
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
    return false; //Implement later
}
bool Package::Save() noexcept
{
    //Header auto-saves
    //Entries auto-saves
    if (!this->index.Write(this->entries))
        return false;

    if (this->IsCompressed())
    {
        if (!this->compressedLocation.has_value())
            return false;

        std::ofstream outFile(*this->compressedLocation, std::ios::trunc);
        return this->Compress(outFile);
    }

    return true;
}
void Package::Close() noexcept
{
    index.Close();
    header.Close();
    entries.clear();
}
void Package::DisplayContents(std::ostream& out) const noexcept
{
    for (const auto& item : entries)
    {
        out << item.GetIndex().Name() << " @ " << item.GetIndex().Key() << ": ";
        (void)item.DisplayData(out);
        out << '\n';
    }
}

const PackageEntry& Package::ResolveEntry(const std::string& entryName) const
{
    auto iter = std::find_if(this->entries.begin(), this->entries.end(), [&entryName](const PackageEntry& obj) -> bool
    {
        return obj.GetIndex().Name() == entryName;
    });

    if (iter == this->entries.end())
        throw std::logic_error("Cannot find an entry with that name.");
    else
        return iter.operator*();
}
PackageEntry& Package::ResolveEntry(const std::string& entryName)
{
    return const_cast<PackageEntry&>(const_cast<const Package*>(this)->ResolveEntry(entryName));
}
const PackageEntry& Package::ResolveEntry(PackageEntryKey key) const
{
    auto target = GetEntry(key.EntryID);
    if (target == this->entries.end() || key.PackageID != this->packID)
        throw std::logic_error("This entry is not in this package");

    return *target;
}
PackageEntry& Package::ResolveEntry(PackageEntryKey key)
{
    return const_cast<PackageEntry&>(const_cast<const Package*>(this)->ResolveEntry(key));
}

bool Package::LoadAllEntries() noexcept
{
    try
    {
        for (auto& item : entries)
            if (!item.Load())
                return false;
    }
    catch (...)
    {
        return false;
    }

    return true;
}
void Package::UnloadAllEntries() noexcept
{
    for (auto& item : entries)
        (void)item.Unload();
}

bool Package::RemoveEntry(unsigned long ID) noexcept
{
    return this->ReleaseEntry(ID).has_value();
}
void Package::RemoveAllEntries() noexcept
{
    std::filesystem::remove_all(this->VarLocation());
    std::filesystem::create_directory(this->VarLocation());

    this->currID = 0;
    this->entries.clear();

    (void)this->index.Write(this->entries);
}
std::optional<PackageEntry> Package::ReleaseEntry(unsigned long ID) noexcept
{  
    auto iter = this->GetEntry(ID);

    if (iter == this->entries.end())
        return {};
    
    PackageEntry target(std::move(*iter));
    this->entries.erase(iter);

    return target;
}
std::optional<PackageEntryKey> Package::AddEntry(std::string elementName, PackageEntryType type, std::unique_ptr<VariableType>&& data) noexcept
{
    if (type != PackageEntryType::Temporary && name.empty())
        return {};

    PackageEntry result(PackageEntryIndex(PackageEntryKey(this->packID, this->GetNextID()), type, std::move(elementName), 0), this);

    if ( !result.Data(std::move(data)) )
        return {};
    else
        this->entries.emplace_back( std::move(result) );

    return result.GetIndex().Key();
}