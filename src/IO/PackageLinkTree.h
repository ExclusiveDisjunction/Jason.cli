//
// Created by exdisj on 8/12/24.
//

#ifndef JASON_PACKAGELINKTREE_H
#define JASON_PACKAGELINKTREE_H

#include "PackageLinkNode.h"
#include "PackageUtility.h"
#include "PackageHandle.h"

#include <vector>

class PackageLinkTree
{
private:
    PackageLinkNode* root;

public:
    PackageLinkTree();
    explicit PackageLinkTree(PackageHandle&& root, bool enabled, const PackageIndex& index) noexcept;
    PackageLinkTree(const PackageLinkTree& obj) = delete;
    PackageLinkTree(PackageLinkTree&& obj) noexcept;
    ~PackageLinkTree();

    PackageLinkTree& operator=(const PackageLinkTree& obj) = delete;
    PackageLinkTree& operator=(PackageLinkTree&& obj) noexcept;

    [[nodiscard]] PackageLinkNode& GetRoot() const noexcept;
    void InitiateRoot(PackageHandle&& target, bool enabled, const PackageIndex& index) noexcept;
    void Clear() noexcept;

    [[nodiscard]] bool Contains(const std::filesystem::path& target) const noexcept;

    void Flatten(std::vector<PreProcessedPackage*>& result) const noexcept;
};


#endif //JASON_PACKAGELINKTREE_H
