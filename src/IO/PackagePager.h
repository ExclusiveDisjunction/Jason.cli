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

    /// @brief Moves forward one unit for a currently bound entry
    /// @return if the move was not EOF
    bool Advance();
    /// @brief Moves forward one page, for a currently bound entry. Always moves to the front of the page (position zero in that page)
    /// @return If the move was not EOF
    bool AdvancePage();

    /// @brief Sets up page structure
    /// @param allocatedElements The currently loaded elements from the index
    void ReviewKnownElements(const std::vector<class PackageEntry>& allocatedElements);

    /// @brief Reads a single unit bound from the currently bound item.
    /// @return The unit that is read. 
    Unit ReadUnit();
    /// @brief Reads a specific number of units from the currently bound item.
    /// @param Units The number of units to read
    /// @return A list of the units read
    std::vector<Unit> ReadUnits(unsigned int Units);
    /// @brief Read all units from the currently bound item.
    /// @return All elements from the currently bound item.
    std::vector<Unit> ReadAllUnits();
    /// @brief Writes a list of units into the currently bound item. Starts from the current position.
    /// @param units The units to write
    /// @return The status. Fails if the units could not be exhausted
    bool WriteUnits(const std::vector<Unit>& units);
    /// @brief Sets all units in the currently bound item to zero.
    /// @return If the operation was successful
    bool WipeAll();

    /// @brief Allocate a specified number of pages, resizing the pages stored in the index.
    /// @param pages The number of pages needed
    /// @param index The handle to the index 
    /// @return True if the specified number of pages requested were satified
    bool Allocate(unsigned pages, PackageEntryIndex& index);
    /// @brief Allocate a specified number of vectors at the end of the file. Works off of the currently bound entry
    /// @param pages The number of pages to request
    /// @return An empty vector if it failed, or a list of page indexes otherwise
    std::vector<unsigned int> Allocate(unsigned pages);

    /// @brief Binds to a specifc package entry
    /// @param index The index to bind to
    void Bind(PackageEntryIndex& index);
    /// @brief Resets the state of the reader
    void Reset();
    /// @brief Closes the reader
    void Close();
    /// @brief Flushes the stream to the file
    void Flush();
    /// @brief Erases the whole file
    void TruncateFile();

    /// @brief Moves the pager relative to the currently bound entry's locations
    /// @param unitPosition The offset from the start
    /// @return If the position is EOF
    bool MoveRelative(unsigned unitPosition);
    /// @brief Moves absolute relative to the file's total position
    /// @param pagePosition The page location
    /// @param unitPosition The unit position
    /// @return If the location is EOF
    bool MoveAbsolute(unsigned pagePosition, unsigned unitPosition);
    /// @brief Moves absolute relative to the file's total position
    /// @param pagePosition The page location & unit position
    /// @return If the location is EOF
    bool MoveAbsolute(std::pair<unsigned, unsigned> loc);

public:
    PackagePager(FileHandle&& location, unsigned char UnitSize, unsigned PageSize);
    PackagePager(const PackagePager& obj) = delete;
    PackagePager(PackagePager&& obj) noexcept;
    ~PackagePager();

    friend class Package;
    friend class PackageEntry;

    PackagePager& operator=(const PackagePager& obj) = delete;
    PackagePager& operator=(PackagePager&& obj) noexcept;

    /// @brief The currently set unit size (bytes per unit)
    [[nodiscard]] unsigned char UnitSize() const noexcept;
    /// @brief The currently set page size (units per page)
    [[nodiscard]] unsigned PageSize() const noexcept;

    /// @brief If the entry is at the end of its blocks
    /// @return If the entry is at the end of its blocks
    bool EndOfFile() const noexcept;
    /// @brief Determienes if the pager is currently bound to an entry
    /// @return If it is bound to an entry
    bool IsBound() const noexcept;

    /// @brief Gets the position relative to the start of the currently bound entry's position
    /// @return The relative unit position
    unsigned GetRelativePosition();
    /// @brief Gets the absolute position of the file
    /// @return A pair containing the page & unit position
    std::pair<unsigned, unsigned> GetAbsolutePosition();
};


#endif //JASON_PACKAGEPAGER_H
