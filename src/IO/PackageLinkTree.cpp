//
// Created by exdisj on 8/12/24.
//

#include "PackageLinkTree.h"
#include "PackageLink.h"
#include "PackageHandle.h"
#include <utility>

PackageLinkTree::PackageLinkTree() : root(nullptr)
{

}
PackageLinkTree::PackageLinkTree(PackageHandle&& root, bool enabled, const PackageIndex& index) noexcept : PackageLinkTree()
{
    this->InitiateRoot(std::move(root), enabled, index);
}
PackageLinkTree::PackageLinkTree(PackageLinkTree&& obj) noexcept : root(std::exchange(obj.root, nullptr))
{

}
PackageLinkTree::~PackageLinkTree()
{
    Clear();
}

PackageLinkTree& PackageLinkTree::operator=(PackageLinkTree&& obj) noexcept
{
    delete root;
    root = std::exchange(obj.root, nullptr);

    return *this;
}

PackageLinkNode& PackageLinkTree::GetRoot() const noexcept
{
    return *root;
}
void PackageLinkTree::InitiateRoot(PackageHandle&& target, bool enabled, const PackageIndex& index) noexcept
{
    delete root;
    root = new PackageLinkNode(PackageLink(std::move(target), enabled), index);
}
void PackageLinkTree::Clear() noexcept
{
    delete root;
    root = nullptr;
}

bool PackageLinkTree::Contains(const std::filesystem::path& target) const noexcept
{
    return root && root->Contains(target);
}

void PackageLinkTree::Flatten(std::vector<PreProcessedPackage*>& result) const noexcept
{
    root->Flatten(result);
}