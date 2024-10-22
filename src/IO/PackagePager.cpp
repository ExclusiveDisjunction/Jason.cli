//
// Created by exdisj on 10/21/24.
//

#include "PackagePager.h"

PackagePager::PackagePager(std::filesystem::path& location) : handle(location, std::ios::out | std::ios::in | std::ios::binary), binding(), location(std::make_pair(0u, 0u))
{
    handle.file.seekg(0, std::ios::beg);
}
PackagePager::PackagePager(PackagePager&& obj) noexcept : handle(std::move(obj.handle)), binding(std::move(obj.binding)), location(std::move(obj.location))
{

}
PackagePager::~PackagePager()
{
    handle.Close();
    binding = {};
    location = std::make_pair(0u, 0u);
}

PackagePager& PackagePager::operator=(PackagePager&& obj) noexcept
{
    Close();

    this->handle = std::move(obj.handle);
    this->binding = std::move(obj.binding);
    this->location = std::move(obj.location);
    return *this;
}

Unit ReadUnit()
{

}
std::vector<Unit> ReadUnits(int Units)
{

}
std::vector<Unit> ReadAllUnits()
{

}
bool WriteUnits(const std::vector<Unit>& units)
{

}
bool WipeAll()
{

}
