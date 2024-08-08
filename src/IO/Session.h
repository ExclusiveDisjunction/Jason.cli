//
// Created by exdisj on 8/8/24.
//

#ifndef JASON_SESSION_H
#define JASON_SESSION_H

#include <optional>

#include "PackageUtility.h"
#include "Package.h"

class Session
{
private:
    std::vector<Package*> packages;
    std::vector<UnloadedPackage*> unloadedPackages;

    bool FindUserPackage(PackageHandle& usr);

    bool PreProcessPackages(std::vector<PreProcessedPackage>& result);
    bool PreProcessPackage(PreProcessedPackage& target, std::vector<PreProcessedPackage>& resolvedLinks);
    void SiveThroughToLoad(std::vector<PreProcessedPackage>& source); //This function will take all items in 'source' that are marked 'toLoad == false', and put the file handles in this->unloadedPackages.

    bool IndexPackages(std::vector<PreProcessedPackage&> toIndex, std::vector<IndexedPackage>& output);
    bool IndexPackage(PreProcessedPackage& toIndex, IndexedPackage& result);

    bool InflatePackage(std::vector<IndexedPackage>& indexed);
    Package* InflatePackage(IndexedPackage& target);

    bool LoadPackageEntries(Package& pack, bool all = false);

public:
    Session() = default;
    ~Session()
    {
        for (Package*& item : packages)
            delete item;
        for (UnloadedPackage*& item : unloadedPackages)
            delete item;

        packages.clear();
        unloadedPackages.clear();
    }

    bool Initiate();

    std::optional<unsigned long> GetPackageID(std::string& name);
    std::optional<PackageEntryKey> ResolveEntry(std::string& name);

    bool LoadPackage(unsigned long ID);
    bool UnloadPackage(unsigned long ID);
};

#endif //JASON_SESSION_H
