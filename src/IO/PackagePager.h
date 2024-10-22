//
// Created by exdisj on 10/21/24.
//

#ifndef JASON_PACKAGEPAGER_H
#define JASON_PACKAGEPAGER_H

#include "FileHandle.h"
#include "BinaryUnit.h"
#include "PackageEntryIndex.h"

#include <vector>
#include <optional>

class PackagePager
{
private:
    FileHandle handle;
    std::optional<std::vector<unsigned>> binding;
    std::pair<unsigned, unsigned> location;

public:
    PackagePager(std::filesystem::path& location);
    PackagePager(const PackagePager& obj) = delete;
    PackagePager(PackagePager&& obj) noexcept;
    ~PackagePager();

    PackagePager& operator=(const PackagePager& obj) = delete;
    PackagePager& operator=(PackagePager&& obj) noexcept;

    Unit ReadUnit();
    std::vector<Unit> ReadUnits(int Units);
    std::vector<Unit> ReadAllUnits();
    bool WriteUnits(const std::vector<Unit>& units);
    bool WipeAll();

    bool Allocate(unsigned pages, const PackageEntryIndex& index);

    void Bind(const PackageEntryIndex& index);
    void Reset();
    void Close();
    void Flush();

    bool EndOfFile() const noexcept;

    bool MoveRelative(unsigned unitPosition);
    bool MoveAbsolute(unsigned pagePosition, unsigned unitPosition);
    bool MoveAbsolute(std::pair<unsigned, unsigned> loc);
    unsigned GetRelativePosition();
    std::pair<unsigned, unsigned> GetAbsolutePosition();

    bool IsFragmented();
    bool IsFragemented(const PackageEntryIndex& target);
    void Defragment(const std::vector<class PackageEntry>& entries);
};


#endif //JASON_PACKAGEPAGER_H
