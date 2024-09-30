#include "PackageIndex.h"
#include "PackageEntry.h"

PackageIndex::PackageIndex(FileHandle&& handle) : handle(std::move(handle)) 
{

}

std::vector<PackageEntryIndex> PackageIndex::ReadIndex() noexcept
{
    std::vector<PackageEntryIndex> index;

    return index;
}
bool PackageIndex::Write(const std::vector<PackageEntry*>& entries) noexcept
{
    return Write(this->handle.file, entries);
}
bool PackageIndex::Write(std::ostream& out, const std::vector<PackageEntry*>& entries) const noexcept 
{
    return false;
}

void PackageIndex::Close()
{
    this->handle.Close();
}