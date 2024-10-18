//
// Created by exdisj on 8/7/24.
//

#ifndef JASON_PACKAGE_H
#define JASON_PACKAGE_H

#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <optional>
#include <filesystem>

#include "PackageEntry.h"
#include "PackageUtility.h"
#include "FileHandle.h"
#include "PackageHeader.h"
#include "PackageIndex.h"

class Package
{
private:
    enum State
    {
        None = 0,
        Compressed = 1
    };

    Package(unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index);
    Package(std::filesystem::path location, unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index);
    Package(std::filesystem::path uLocation, std::filesystem::path cLocation, unsigned long ID, std::string name, PackageHeader&& header, PackageIndex&& index);

    std::filesystem::path location;
    std::optional<std::filesystem::path> compressedLocation;

    unsigned long packID;
    unsigned long currID = 0;
    unsigned char state;

    std::string name;
    PackageHeader header;
    PackageIndex index;

    std::vector<PackageEntry> entries;

    void IndexEntries();

    [[nodiscard]] std::vector<PackageEntry>::const_iterator GetEntry(unsigned long ID) const noexcept;
    [[nodiscard]] std::vector<PackageEntry>::iterator GetEntry(unsigned long ID) noexcept;

    unsigned long GetNextID() noexcept { return currID++;}

public:
    Package(const Package& obj) = delete;
    Package(Package& obj) noexcept = delete;
    ~Package();

    friend class Session;

    Package& operator=(const Package& obj) = delete;
    Package& operator=(Package&& obj) noexcept = delete;

    [[nodiscard]] static std::unique_ptr<Package> OpenFromDirectory(std::filesystem::path& dir, unsigned long ID);
    [[nodiscard]] static std::unique_ptr<Package> OpenFromCompressed(std::filesystem::path& pack, std::filesystem::path& targetDir, unsigned long ID);
    [[nodiscard]] static std::unique_ptr<Package> NewPackage(const std::string& name, const std::filesystem::path& landingDirectory, unsigned long ID);

    [[nodiscard]] const std::filesystem::path& Location() const noexcept;
    [[nodiscard]] std::filesystem::path VarLocation() const noexcept;
    [[nodiscard]] unsigned long GetID() const noexcept;
    [[nodiscard]] const std::string& GetName() const noexcept;
    [[nodiscard]] bool IsCompressed() const noexcept;
    [[nodiscard]] const PackageHeader& Header() const noexcept;
    [[nodiscard]] PackageHeader& Header() noexcept;

    [[nodiscard]] bool Compress(std::ostream& out) const noexcept;
    [[nodiscard]] bool Save() noexcept;
    void Close() noexcept;
    void DisplayContents(std::ostream& out) const noexcept;

    [[nodiscard]] const PackageEntry& ResolveEntry(const std::string& name) const;
    [[nodiscard]] PackageEntry& ResolveEntry(const std::string& name);
    [[nodiscard]] const PackageEntry& ResolveEntry(PackageEntryKey key) const;
    [[nodiscard]] PackageEntry& ResolveEntry(PackageEntryKey key);
    [[nodiscard]] bool DoesEntryExist(unsigned long ID) noexcept;

    [[nodiscard]] bool LoadAllEntries() noexcept;
    void UnloadAllEntries() noexcept;

    [[nodiscard]] bool RemoveEntry(unsigned long ID) noexcept; //Removes from the internal list & deletes it.
    void RemoveAllEntries() noexcept;
    [[nodiscard]] PackageEntry ReleaseEntry(unsigned long ID) noexcept; //Removes from the internal list, but does not delete it.
    [[nodiscard]] std::optional<PackageEntryKey> AddEntry(std::string name, PackageEntryType type, std::unique_ptr<VariableType>&& data) noexcept;
};

#endif //JASON_PACKAGE_H
