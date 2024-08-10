//
// Created by exdisj on 8/8/24.
//

#ifndef JASON_SESSION_H
#define JASON_SESSION_H

#include <optional>
#include <filesystem>

#include "PackageUtility.h"
#include "Package.h"

class Session
{
private:
    std::vector<Package*> packages;
    std::vector<UnloadedPackage*> unloadedPackages;
    unsigned long currID;
    bool initiated = false;

    [[nodiscard]] unsigned long GetNextID() noexcept { return currID++; }

    [[nodiscard]] static std::optional<PackageHandle> FindUserPackage();

    [[nodiscard]] bool PreProcessPackages(PackageHandle& usr, std::vector<PreProcessedPackage*>& result);
    [[nodiscard]] static PreProcessedPackage* PreProcessPackage(PackageHandle& target);

    [[nodiscard]] static bool ExtractLinks(PreProcessedPackage& target, std::vector<std::pair<PackageHandle, bool>>& result);

    [[nodiscard]] bool IndexPackages(std::vector<PreProcessedPackage*>& toIndex, std::vector<IndexedPackage*>& output); //Note that the ProjectHandles that were in toIndex are now in the IndexedPackage, and all entries have been deleted.
    [[nodiscard]] static IndexedPackage* IndexPackage(PreProcessedPackage*& toIndex);

    [[nodiscard]] bool InflatePackages(std::vector<IndexedPackage*>& indexed);
    [[nodiscard]] Package* InflatePackage(IndexedPackage*& target);

    [[nodiscard]] bool LoadInflatedEntries();

    [[nodiscard]] const Package* ResolvePackage(const std::string& name) const noexcept;
    [[nodiscard]] Package* ResolvePackage(const std::string& name) noexcept;

    void LoadFromPreviousPackages(const std::filesystem::path& hostDir);

public:
    Session();
    ~Session();

    [[nodiscard]] bool Initiate();
    [[nodiscard]] bool CheckForUnclosedPackages() noexcept; //If this returns true, exit the program.
    [[nodiscard]] bool Save(bool DeleteDir = false) noexcept;
    [[nodiscard]] bool Close();
    void ForceClose();

    [[nodiscard]] std::optional<unsigned long> GetPackageID(const std::string& name) const noexcept;
    [[nodiscard]] std::optional<PackageEntryKey> ResolveEntry(const std::string& name) const noexcept;

    [[nodiscard]] bool LoadPackage(unsigned long ID);
    [[nodiscard]] bool UnloadPackage(unsigned long ID);
};

#endif //JASON_SESSION_H
