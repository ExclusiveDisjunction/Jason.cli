//
// Created by exdisj on 8/7/24.
//

#include "Package.h"

Package::Package(std::string dir_path, PackageHandle& pack, PackageHandle& links, PackageHandle& header, unsigned long ID) : dir_path(std::move(dir_path)), pack(std::move(pack)), links(std::move(links)), header(std::move(header)), PackID(ID), locked(false), CurrID(0)
{

}

Package::~Package()
{
    Close();
}

bool Package::Save() noexcept
{
    //TODO: Package Saving
    return true;
}
void Package::Close() noexcept
{
    //ONLY leaves the 'pack' handle and the 'dir_path'.

    links.Close();
    header.Close();
    entries.clear();

    CurrID = 0;
    author = {};
    locked = false;
    ver = {};
    name.clear();
}

bool Package::LoadEntries(bool all) noexcept
{
    bool result = true;
    for (PackageEntry& ent : entries)
    {
        if ((ent.LoadImm() || all) && !ent.IsTemporary())
        {
            std::ifstream target(ent.GetPath(this->dir_path));
            if (!target)
            {
                std::cout << "Entry Loading: Could not load entry '" << ent.Name() << "' because it's unpacked file could not be accessed.\n";
                result &= false;
            }

            bool tmp = ent.ReadFromFile(target);
            if (!tmp)
                std::cout << "Entry Loading: Could not load entry '" << ent.Name() << "' because the data in the file was not of proper format.\n";

            result &= tmp;
        }
    }

    return result;
}