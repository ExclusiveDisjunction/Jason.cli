//
// Created by exdisj on 8/12/24.
//

#ifndef JASON_PACKAGELINKNODE_H
#define JASON_PACKAGELINKNODE_H

#include "PackageLink.h"
#include "PackageUtility.h"

class PackageLinkNode
{
private:
    PackageLink link;
    PackageIndex index;

    std::vector<PackageLinkNode*> children;
public:
    PackageLinkNode(PackageLink&& link, const PackageIndex& index);
    ~PackageLinkNode();

    PackageLinkNode& AddChild(PackageLink&& packageLink, const PackageIndex& packageIndex) noexcept;

    PackageLink& GetLink() noexcept;
    const PackageIndex& GetIndex() const noexcept;

    bool Contains(const std::filesystem::path& target) const noexcept;
    void Flatten(std::vector<PreProcessedPackage*>& result) noexcept;
};


#endif //JASON_PACKAGELINKNODE_H
