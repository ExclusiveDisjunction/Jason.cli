//
// Created by exdisj on 10/21/24.
//

#ifndef JASON_PACKAGEPAGER_H
#define JASON_PACKAGEPAGER_H

#include "FileHandle.h"
#include "PackageEntryIndex.h"
#include "PackageEntryKey.h"
#include "../Core/BinaryUnit.h"

#include <vector>
#include <optional>
#include <unordered_map>
#include <queue>

struct PackagePagerBinding 
{
private:
    PackagePagerBinding(PackageEntryKey key, unsigned offset) : key(key), offset(offset) {}

    PackageEntryKey key;
    unsigned offset;
    bool eof = false;

    friend class PackagePager;
};

class PackagePager
{
private:
    FileHandle handle;
    FileHandle index;

    std::optional<PackagePagerBinding> binding;
    std::unordered_map<PackageEntryKey, std::vector<unsigned>> locations;
    std::priority_queue<unsigned, std::vector<unsigned>,  std::greater<unsigned>> open_pages;
    
    /*
        Open units handling

        1. The index contains an "OPEN" section that lists the open pages
        2. The index function is to read this section, so that the data can be loaded.
        3. The open_units should be stored in the index file, so that it can be restored.

        If a unit is added, it will take from the open_units as it depletes.
        If a unit is removed, it will add to the open_units all of its allocated units. 
    */

    unsigned abs_location;
    unsigned char unit_size;

    /// @brief Moves forward one unit for a currently bound entry
    bool Advance();

    /// @brief Reads from the locations index to determine where all elements are.
    bool ReadIndex();
    bool SaveIndex();

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

    std::vector<unsigned> RequestUnits(unsigned units);
    /// @brief Allocate a specified number of units, resizing the pages stored in the index.
    /// @param pages The number of pages needed
    /// @param index The handle to the index 
    /// @return True if the specified number of units requested were satified
    bool Allocate(unsigned units, PackageEntryKey key);
    bool AllocateOnBound(unsigned units);
    bool Register(PackageEntryKey key);
    /// @brief Removes the key from the index, and unallocates its pages, so that it will not be considered in the binary data anymore
    /// @param key The object to remove
    void RemoveAllocation(PackageEntryKey key);
    /// @brief Ensures that the currently bound element has a specific number of elements allocated
    bool EnsureAllocation(unsigned needed);

    /// @brief Binds to a specifc package entry
    /// @param index The index to bind to
    /// @returns False if the binding results in a miss from the index.
    bool Bind(PackageEntryKey key);
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
    bool MoveRelative(unsigned offset);
    /// @brief Moves absolute relative to the file's total position
    /// @param unitPosition The unit position, absolute to the file start
    /// @return If the location is not EOF
    bool MoveAbsolute(unsigned unitPosition);

public:
    PackagePager(FileHandle&& location, FileHandle&& index_loc, unsigned char unit_size);
    PackagePager(const PackagePager& obj) = delete;
    PackagePager(PackagePager&& obj) noexcept;
    ~PackagePager();

    friend class Package;
    friend class PackageEntry;

    PackagePager& operator=(const PackagePager& obj) = delete;
    PackagePager& operator=(PackagePager&& obj) noexcept;

    /// @brief The currently set unit size (bytes per unit)
    [[nodiscard]] unsigned char UnitSize() const noexcept;

    /// @brief If the entry is at the end of its blocks
    /// @return If the entry is at the end of its blocks
    bool IsEOF() const noexcept;
    /// @brief Determienes if the pager is currently bound to an entry
    /// @return If it is bound to an entry
    bool IsBound() const noexcept;

    /// @brief Gets the position relative to the start of the currently bound entry's position
    /// @return The relative unit position
    std::optional<unsigned> GetRelativePosition() const noexcept;
    /// @brief Gets the absolute position of the file
    /// @return A pair containing the page & unit position
    unsigned GetAbsolutePosition() const noexcept;
};


#endif //JASON_PACKAGEPAGER_H
