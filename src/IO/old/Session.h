//
// Created by exdisj on 8/8/24.
//

#ifndef JASON_SESSION_H
#define JASON_SESSION_H

#include <optional>
#include <filesystem>

#include "PackageUtility.h"
#include "Package.h"
#include "PackageLinkTree.h"
#include "PackageHandle.h"
#include "UnloadedPackage.h"

class Session
{
private:
    std::vector<Package*> packages;
    std::vector<UnloadedPackage*> unloadedPackages;
    unsigned long currID;
    bool initiated = false;

    [[nodiscard]] unsigned long GetNextID() noexcept { return currID++; }

    [[nodiscard]] static std::optional<FileHandle> FindUserPackage();

    [[nodiscard]] static bool GetLinksTree(PackageLinkTree& tree, FileHandle&& usr);
    [[nodiscard]] static bool FillLinkTree(PackageLinkNode& parent, const PackageLinkTree& tree) noexcept;
    [[nodiscard]] static std::optional<PackageIndex> GetPackageIndex(FileHandle& target) noexcept;
    [[nodiscard]] static bool ExtractLinks(PackageLinkNode& target, std::vector<PackageLink>& result);

    [[nodiscard]] bool InflatePackages(std::vector<PreProcessedPackage*>& indexed);
    [[nodiscard]] Package* InflatePackage(PreProcessedPackage*& target);

    [[nodiscard]] bool IndexPackageEntries(); //Note that the ProjectHandles that were in toIndex are now in the IndexedPackage, and all entries have been deleted.

    [[nodiscard]] std::vector<Package*>::const_iterator ResolvePackage(const std::string& name) const noexcept;
    [[nodiscard]] std::vector<UnloadedPackage*>::const_iterator ResolveUnloadedPackage(const std::string& name) const noexcept;
    [[nodiscard]] std::vector<Package*>::const_iterator ResolvePackage(unsigned long ID) const noexcept;
    [[nodiscard]] std::vector<UnloadedPackage*>::const_iterator ResolveUnloadedPackage(unsigned long ID) const noexcept;
    [[nodiscard]] std::vector<Package*>::iterator ResolvePackage(unsigned long ID) noexcept;
    [[nodiscard]] std::vector<UnloadedPackage*>::iterator ResolveUnloadedPackage(unsigned long ID) noexcept;

    void LoadFromPreviousPackages(const std::filesystem::path& hostDir);

public:
    Session();
    ~Session();

    [[nodiscard]] static std::string MakePackageName(const std::filesystem::path& target) noexcept;

    [[nodiscard]] bool Initiate();
    [[nodiscard]] bool CheckForUnclosedPackages() noexcept; //If this returns true, exit the program.
    [[nodiscard]] bool Save(bool DeleteDir = false) noexcept;
    [[nodiscard]] bool Close();
    void ForceClose();

    [[nodiscard]] std::optional<unsigned long> GetPackageID(const std::string& name) const noexcept;
    [[nodiscard]] std::optional<PackageEntryKey> ResolveEntry(const std::string& name) const noexcept;

    [[nodiscard]] bool IsPackageLoaded(unsigned long ID) const noexcept;
    [[nodiscard]] bool LoadPackage(unsigned long ID) noexcept;
    [[nodiscard]] bool UnloadPackage(unsigned long ID) noexcept;
};

#endif //JASON_SESSION_H
