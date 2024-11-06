//
// Created by exdisj on 8/7/24.
//

#include <algorithm>

#include "Package.h" 
#include "PackageEntryIndex.h"
#include "UnloadedPackage.h"
#include "../Core/Common.h"

using namespace std::filesystem;

Package::Package(unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index) : packID(ID), name(std::move(name)), header(std::move(header)), index(std::move(index)), state(0), ref(new PackageReference(*this))
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

void Package::IndexEntries()
{
    std::vector<PackageEntryIndex> indexes = this->index.ReadIndex(this->packID);
    if (indexes.empty())
        return; 

    for (PackageEntryIndex& obj : indexes)
    {
        this->currID = std::max(this->currID, obj.Key().EntryID);

        PackageEntry& result = this->entries.emplace_back(std::move(obj), std::weak_ptr<PackageReference>(this->ref));
        
        std::string errorMessage;
        if (result.GetIndex().LoadImmediate() && result.LoadNoThrow(errorMessage))
            throw std::logic_error(std::string("For entry = ") + this->name + "::" + result.GetIndex().Name() + ", the data could not be loaded and the Load Idemedatley flag was set. Could not load because '" + errorMessage + "'");
    }
    //We need to increment the currID, because right now it has the highest value of the current key.
    this->currID++;
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

std::shared_ptr<Package> Package::OpenFromDirectory(const std::filesystem::path& dir, unsigned long ID)
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
    FileHandle header_f(header_l), index_f(index_l);
    if (!std::filesystem::exists(var_l))
        throw std::logic_error("The variable folder does not exist");

    PackageHeader header(std::move(header_f));
    PackageIndex index(std::move(index_f));

    std::shared_ptr<Package> result( new Package(dir, ID, dir.filename(), std::move(header), std::move(index)) );
    result->IndexEntries();
    
    return result;
}
std::shared_ptr<Package> Package::OpenFromCompressed(const std::filesystem::path& pack, const std::filesystem::path& targetDir, unsigned long ID)
{
    throw std::logic_error("Not implemented yet.");
}
std::shared_ptr<Package> Package::OpenFromUnloaded(const UnloadedPackage& obj)
{
    return OpenFromDirectory(obj.Target, obj.PackID);
}
std::shared_ptr<Package> Package::NewPackage(const std::string& name, const std::filesystem::path& landingDirectory, unsigned long ID)
{
    if (!std::filesystem::exists(landingDirectory) || !std::filesystem::is_directory(landingDirectory) || name.empty())
        throw std::logic_error("Landing directory is not a directory, does not exist, or the name is empty");

    std::filesystem::path path = landingDirectory / name;
    if (std::filesystem::exists(path)) //Already exists
        return OpenFromDirectory(std::move(path), ID);

    if (!std::filesystem::create_directory(path))
        throw std::logic_error("Unable to create directory");

    std::filesystem::path header_p = path / "header", index_p = path / "index", entries_p = path / "var";

    if (!std::filesystem::create_directory(entries_p))
        throw std::logic_error("Unable to create entries directory");

    FileHandle header_f(header_p), index_f(index_p);

    PackageHeader header(std::move(header_f));
    PackageIndex index(std::move(index_f));
    if (!header.Write())
        throw std::logic_error("The header could not be written to the file system"); 

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
            item.Load();
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

    PackageEntry result(
        PackageEntryIndex(
                PackageEntryKey(this->packID, 
                                this->GetNextID()),
                                type, 
                                std::move(elementName), 
                                0),
        std::weak_ptr<PackageReference>(ref));

    if ( !result.Data(std::move(data)) )
        return {};
    else
        this->entries.emplace_back( std::move(result) );

    return result.GetIndex().Key();
}