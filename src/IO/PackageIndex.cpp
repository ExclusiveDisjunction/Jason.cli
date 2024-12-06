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

    while (!in.eof())
    {
        std::string line_r;
        std::getline(in, line_r);
        if (line_r.empty())
            break;

        std::stringstream line(line_r);
        line_r.clear();

        PackageEntryIndex& thisTarget = index.emplace_back();
        thisTarget.key.PackageID = PackageID;
        line >> thisTarget;
    }

    return index;
}
bool PackageIndex::Write(const std::vector<PackageEntry>& entries) noexcept
{
    this->handle.file.close();
    this->handle.file.open(this->handle.path, std::ios::in | std::ios::out | std::ios::trunc);

    return Write(this->handle.file, entries);
}
bool PackageIndex::Write(std::ostream& out, const std::vector<PackageEntry>& entries) noexcept
{
    out.seekp(0, std::ios::beg);

    for (auto& obj : entries)
        out << obj.GetIndex() << '\n';

    out.flush();
    return !out.bad();
}

void PackageIndex::Close()
{
    this->handle.Close();
}
void PackageIndex::TruncateFile()
{
    std::filesystem::path this_path = this->handle.path;
    this->Close();

    try
    {
        this->handle.Open(this_path, std::ios::in | std::ios::out | std::ios::trunc);
    }
    catch (...)
    {
        //TODO: Add logging.
    }
    
}