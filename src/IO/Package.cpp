//
// Created by exdisj on 8/7/24.
//

#include "Package.h"
#include "../Common.h"
#include "PackageHandle.h"
#include "Session.h"

Package::Package(std::filesystem::path location, FileHandle&& index, FileHandle&& header_l, unsigned long ID, std::string name, PackageHeader&& header) : location(std::move(location)), index_l(std::move(index)), header_l(std::move(header_l)), packID(ID), name(std::move(name)), header(std::move(header))
{
    
}
Package::~Package()
{
    Close();
}

void Package::Close() noexcept
{
    //ONLY leaves the 'pack' handle and the 'dir_path'.

    index_l.file.close();
    header_l.file.close();

    for (auto& item : entries)
    {
        item->parent = nullptr;
        delete item;
        item = nullptr;
    }
    entries.clear();
}