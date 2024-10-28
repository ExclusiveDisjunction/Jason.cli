//
// Created by exdisj on 8/7/24.
//

#include <algorithm>

#include "Package.h" 
#include "PackageEntryIndex.h"
#include "../Common.h"

Package::Package(unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index) : packID(ID), name(std::move(name)), header(std::move(header)), index(std::move(index)), state(0)
{

}
Package::Package(std::filesystem::path location, unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index) : Package(ID, std::move(name), std::move(header), std::move(index))
{
    this->location = std::move(location);
    this->compressedLocation = {};
}
Package::Package(std::filesystem::path uLocation, std::filesystem::path cLocation, unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index) : Package(ID, std::move(name), std::move(header), std::move(index))
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

        PackageEntry& result = this->entries.emplace_back(std::move(obj), this);
        
        if (result.GetIndex().LoadImmediate() && !result.Load())
            throw std::logic_error("For entry = " + this->name + "::" + result.GetIndex().Name() + ", the data could not be loaded and the Load Idemedatley flag was set");
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

std::optional<std::shared_ptr<Package>> Package::OpenFromDirectory(std::filesystem::path& dir, unsigned long ID) noexcept
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

    try
    {
        FileHandle header_f(header_l), index_f(index_l);
        if (!std::filesystem::exists(var_l))
            return {};

        PackageHeader header(std::move(header_f), JASON_CURRENT_VERSION);
        PackageIndex index(std::move(index_f));

        if (!header.Read())
            return {};

        std::shared_ptr<Package> result( new Package(dir, ID, dir.filename(), std::move(header), std::move(index)) );
        result->IndexEntries();
        return result;
    }
    catch (...)
    {
        return {};
    }
}
std::optional<std::shared_ptr<Package>> Package::OpenFromCompressed(std::filesystem::path& pack, std::filesystem::path& targetDir, unsigned long ID)
{
    throw std::logic_error("Not implemented yet.");

    return OpenFromDirectory(targetDir, ID);
}
std::optional<std::shared_ptr<Package>> Package::NewPackage(const std::string& name, const std::filesystem::path& landingDirectory, unsigned long ID) noexcept
{
    if (!std::filesystem::exists(landingDirectory) || !std::filesystem::is_directory(landingDirectory) || name.empty())
        return {}; //throw std::logic_error("Landing directory is not a directory, does not exist, or the name is empty.");

    std::filesystem::path path = landingDirectory / name;
    if (std::filesystem::exists(path)) //Already exists
        return OpenFromDirectory(path, ID);

    if (!std::filesystem::create_directory(path))
        return {};

    std::filesystem::path header_p = path / "header", index_p = path / "index", entries_p = path / "var";

    if (!std::filesystem::create_directory(entries_p))
        return {};

    try
    {
        FileHandle header_h(header_p, std::ios::trunc | std::ios::in | std::ios::out),
                   index_h(index_p, std::ios::trunc | std::ios::in | std::ios::out);

        PackageHeader header(std::move(header_h), JASON_CURRENT_VERSION);
        PackageIndex index(std::move(index_h));
        if (!header.Write())
            throw std::logic_error("");

        return std::shared_ptr<Package>( new Package(path, ID, name, std::move(header), std::move(index)) );
    }
    catch (std::logic_error& e)
    {
        return {};
    }
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
    if (!PackageIndex::Write(out, this->entries))
        return false;
    out << "\ndata\n";

    for (const auto& entry : this->entries)
        out << entry.GetIndex().Key().EntryID << ' ' << entry << '\n';

    return out.good();
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