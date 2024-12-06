//
// Created by exdisj on 8/7/24.
//

#include <algorithm>

#include "Package.h" 
#include "PackageEntryIndex.h"
#include "UnloadedPackage.h"
#include "../Core/Common.h"

using namespace std::filesystem;

Package::Package(unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index, PackagePager&& pager) : packID(ID), name(std::move(name)), header(std::move(header)), index(std::move(index)), state(0), ref(new PackageReference(*this)), pager(std::move(pager))
{

}
Package::Package(path location, unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index, PackagePager&& pager) : Package(ID, std::move(name), std::move(header), std::move(index), std::move(pager))
{
    this->location = std::move(location);
    this->compressedLocation = {};
}
Package::Package(path uLocation, path cLocation, unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index, PackagePager&& pager) : Package(ID, std::move(name), std::move(header), std::move(index), std::move(pager))
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
    if (!this->pager.ReadIndex())
        throw std::logic_error("could not read pager index");

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

    std::filesystem::path header_l(dir / "header"), index_l(dir / "index"), pager_l(dir / "var"), pager_i_l(dir / "var_index");
    FileHandle header_f(header_l), index_f(index_l), pager_f(pager_l), pager_i(pager_i_l);

    PackageHeader header(std::move(header_f));
    if (!header.Read())
        throw std::logic_error("Could not read header");

    PackageIndex index(std::move(index_f));
    PackagePager pager(std::move(pager_f), std::move(pager_i), sizeof(double));

    std::shared_ptr<Package> result( new Package(dir, ID, dir.filename(), std::move(header), std::move(index), std::move(pager) ));
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

    std::filesystem::path header_p = path / "header", index_p = path / "index", pager_p = path / "var", pager_i_p = path / "var_index";

    auto flags = std::ios::in | std::ios::out | std::ios::trunc;
    FileHandle header_f(header_p, flags), index_f(index_p, flags), pager_f(pager_p, flags), pager_i(pager_i_p, flags);

    PackageHeader header(std::move(header_f));
    PackageIndex index(std::move(index_f));
    PackagePager pager(std::move(pager_f), std::move(pager_i), sizeof(double));
    if (!header.Write())
        throw std::logic_error("The header could not be written to the file system"); 

    return std::shared_ptr<Package>( new Package(path, ID, std::move(name), std::move(header), std::move(index), std::move(pager)) );
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
PackagePager& Package::Pager() noexcept 
{
    return this->pager;
}

bool Package::Compress(std::ostream& out) const noexcept
{
    return false; //Implement later
}
bool Package::Save() noexcept
{
    //Header auto-saves
    bool result = true;
    
    if (!this->pager.SaveIndex())
        return false;

    for (auto& entry : this->entries)
    {
        if (entry.IsModified())
            result &= entry.WriteData();
    }

    result &= this->index.Write(this->entries);

    if (this->IsCompressed())
    {
        if (!this->compressedLocation.has_value())
            return false;

        std::ofstream outFile(*this->compressedLocation, std::ios::trunc);
        return this->Compress(outFile);
    }

    this->pager.Reset();
    this->pager.Flush();

    return result;
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
    this->pager.TruncateFile();
    this->index.TruncateFile();

    this->currID = 0;
    this->entries.clear();
}
std::optional<PackageEntry> Package::ReleaseEntry(unsigned long ID) noexcept
{  
    auto iter = this->GetEntry(ID);

    if (iter == this->entries.end())
        return {};
    
    PackageEntry target(std::move(*iter));
    this->entries.erase(iter); //Removes from our data

    //Note that no matter what, after this point, the item is removed from our data. 

    //We will attempt to load the data, because what good is the object if there is no data?
    if (!target.IsLoaded()) 
    {
        try { target.Load(); }
        catch (...) {  } //Could not read, so will return nothing later on. We still have to clean stuff up, so we can't exit yet.
    }

    this->pager.RemoveAllocation(target.index.Key()); //Free space in the pager
    target.parent = {}; //Remove references to this as the parent, so it cannot read or write to file.

    if (!target.IsLoaded()) //Previous load failed
        return {};
    else
        return target;
}
std::optional<PackageEntryKey> Package::AddEntry(std::string elementName, PackageEntryType type, std::unique_ptr<VariableType>&& data) noexcept
{
    if (type != PackageEntryType::Temporary && name.empty())
        return {};

    PackageEntryKey key(this->packID, this->currID + 1); //NOTE: If this function succedes, we need to truly increment currID. This is in case we fail, we can keep that key slot open.
    unsigned needed_pages = !data ? 1 : data->RequiredUnits();
    if (!this->pager.Register(key))
        return {};

    if (!this->pager.Bind(key) || !this->pager.EnsureAllocation(needed_pages))
        return {};

    PackageEntry result(
        PackageEntryIndex(
                key,
                type, 
                std::move(elementName), 
                0,
                (!data ? VT_None : data->GetType())
            ),
        std::weak_ptr<PackageReference>(ref));

    result.Data(std::move(data));
    this->entries.emplace_back( std::move(result) );

    (void)this->GetNextID(); //Truly increment the ID

    return result.GetIndex().Key();
}