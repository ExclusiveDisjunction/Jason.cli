//
// Created by exdisj on 8/7/24.
//

#include "Package.h"
#include "../Common.h"
#include "PackageHandle.h"
#include "Session.h"

Package::Package(std::string dir_path, FileHandle&& pack, FileHandle&& links, unsigned long ID, const PackageIndex& index) : dir_path(std::move(dir_path)), pack(std::move(pack)), links(std::move(links)), PackID(ID), locked(false), CurrID(0), index(index)
{
    this->name = Session::MakePackageName(this->pack.path);
}

Package::~Package()
{
    Close();
}

bool Package::Save() noexcept
{
    //TODO: Package Saving

    return WriteSchematic();
}
bool Package::WriteSchematic() const noexcept
{
    std::ofstream file(this->dir_path / "schema");
    if (!file)
        return false;

    file << this->pack.path.string() << '\n';

    bool result = true;
    for (const auto& ent : entries)
    {
        if (ent.GetType() == Temporary)
            continue;
        result &= ent.WriteSchematic(file);
        file << '\n';
    }

    return result;
}
void Package::Close() noexcept
{
    //ONLY leaves the 'pack' handle and the 'dir_path'.

    links.Close();
    entries.clear();

    CurrID = 0;
    author = {};
    locked = false;
    ver = {};
    name.clear();
}

bool Package::IndexEntries()
{
    std::fstream& file = this->pack.file;
    if (!file)
        return false;

    file.seekg(this->index.entries);

    std::string curr;
    file >> curr; //bypass "entry" section.

    while (file.tellg() <= this->index.functions)
    {
        std::getline(file, curr);

        trim(curr);
        if (curr.empty())
            continue;
        else if (curr == "functions")
            break;

        try
        {
            auto ID = this->GetNextID();
            auto path = this->dir_path / std::to_string(ID);
            std::ofstream out(path, std::ios::trunc | std::ios::out);
            if (!out)
                return false;

            this->entries.emplace_back(curr, out, PackageEntryKey(this->PackID, ID));
            out.close();
        }
        catch (std::logic_error& e)
        {
            std::cerr << "Package Indexing Error: Could not index a variable in package at '" << this->pack.path << "'." << std::endl;
            return false;
        }
    }

    return this->WriteSchematic();
}