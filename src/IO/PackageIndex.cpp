#include "PackageIndex.h"
#include "PackageEntry.h"

#include <sstream>

PackageIndex::PackageIndex(FileHandle&& handle) : handle(std::move(handle)) 
{

}

std::vector<PackageEntryIndex> PackageIndex::ReadIndex(unsigned long PackageID) noexcept
{
    std::vector<PackageEntryIndex> index;

    std::istream& in = this->handle.file;
    in.seekg(0, std::ios::beg);

    while (in.good() && !in.eof())
    {
        std::string line_r;
        std::getline(in, line_r);
        std::stringstream line(line_r);
        line_r.clear();

        PackageEntryIndex& thisTarget = index.emplace_back();
        thisTarget.key.PackageID = PackageID;
        line >> thisTarget;
    }

    return index;
}
bool PackageIndex::Write(const std::vector<PackageEntry*>& entries) noexcept
{
    return Write(this->handle.file, entries);
}
bool PackageIndex::Write(std::ostream& out, const std::vector<PackageEntry*>& entries) noexcept
{
    for (PackageEntry* obj : entries)
    {
        const PackageEntryIndex& index = obj->GetIndex();
        out << index << '\n';
    }

    out.flush();
    return out.good();
}

void PackageIndex::Close()
{
    this->handle.Close();
}