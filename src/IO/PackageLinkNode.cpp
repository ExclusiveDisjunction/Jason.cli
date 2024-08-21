//
// Created by exdisj on 8/12/24.
//

#include "PackageLinkNode.h"


PackageLinkNode::PackageLinkNode(PackageLink&& link, const PackageIndex& index) : link(std::move(link)), index(index)
{

}
PackageLinkNode::~PackageLinkNode()
{
    for (auto& item : this->children)
        delete item;

    children.clear();
}

PackageLinkNode& PackageLinkNode::AddChild(PackageLink&& packageLink, const PackageIndex& packageIndex) noexcept
{
    this->children.push_back(new PackageLinkNode(std::move(packageLink), packageIndex));
    return *this->children.back();
}
PackageLink& PackageLinkNode::GetLink() noexcept
{
    return this->link;
}
const PackageIndex& PackageLinkNode::GetIndex() const noexcept
{
    return this->index;
}

bool PackageLinkNode::Contains(const std::filesystem::path& target) const noexcept
{
    if (this->link.GetPath() == target)
        return true;
    else
    {
        for (const auto& item : this->children)
            if (item->link.GetPath() == target)
                return true;

        return false;
    }
}
void PackageLinkNode::Flatten(std::vector<PreProcessedPackage*>& result) noexcept
{
    for (auto& item : this->children)
        item->Flatten(result);

    result.push_back(new PreProcessedPackage(std::move(this->link), this->index));
}