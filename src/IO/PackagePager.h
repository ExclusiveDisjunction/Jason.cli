//
// Created by exdisj on 10/21/24.
//

#ifndef JASON_PACKAGEPAGER_H
#define JASON_PACKAGEPAGER_H

#include "FileHandle.h"
#include "PackageEntryIndex.h"
#include "../Core/BinaryUnit.h"

#include <vector>
#include <optional>

class PackagePager
{
private:
    FileHandle handle;
    std::optional<std::vector<unsigned int>*> binding;
    std::optional<unsigned long> boundPageIndex;
    std::pair<unsigned, unsigned> location;
    std::unordered_map<unsigned, bool> knownPages;

    bool boundEof = false;

    unsigned char unitSize;
    unsigned pageSize;

public:
    PackagePager(FileHandle&& location, unsigned char UnitSize, unsigned PageSize);
    PackagePager(const PackagePager& obj) = delete;
    PackagePager(PackagePager&& obj) noexcept;
    ~PackagePager();

    void ReviewKnownElements(const std::vector<class PackageEntry>& allocatedElements);

    PackagePager& operator=(const PackagePager& obj) = delete;
    PackagePager& operator=(PackagePager&& obj) noexcept;

    [[nodiscard]] [[maybe_unused]] unsigned char UnitSize() const noexcept;
    [[nodiscard]] [[maybe_unused]] unsigned PageSize() const noexcept;

    Unit ReadUnit();
    bool Advance();
    bool AdvancePage();
    std::vector<Unit> ReadUnits(unsigned int Units);
    std::vector<Unit> ReadAllUnits();
    bool WriteUnits(const std::vector<Unit>& units);
    bool WipeAll();

    /// @brief Allocate a specified number of pages, resizing the pages stored in the index.
    /// @param pages The number of pages needed
    /// @param index The handle to the index 
    /// @return True if the specified number of pages requested were satified
    bool Allocate(unsigned pages, PackageEntryIndex& index);
    /// @brief Allocate a specified number of vectors at the end of the file
    /// @param pages The number of pages to request
    /// @return An empty vector if it failed, or a list of page indexes otherwise
    std::vector<unsigned int> Allocate(unsigned pages);

    void Bind(PackageEntryIndex& index);
    void Reset();
    void Close();
    void Flush();

    bool EndOfFile() const noexcept;
    bool IsBound() const noexcept;

    bool MoveRelative(unsigned unitPosition);
    bool MoveAbsolute(unsigned pagePosition, unsigned unitPosition);
    bool MoveAbsolute(std::pair<unsigned, unsigned> loc);
    unsigned GetRelativePosition();
    std::pair<unsigned, unsigned> GetAbsolutePosition();

    //bool IsFragmented();
    //bool IsFragemented(const PackageEntryIndex& target);
    //void Defragment(const std::vector<class PackageEntry>& entries);
};


#endif //JASON_PACKAGEPAGER_H
