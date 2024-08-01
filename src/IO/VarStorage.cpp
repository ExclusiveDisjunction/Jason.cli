//
// Created by exdisj on 5/26/24.
//

#include "VarStorage.h"

#include <utility>

VarStorage::VarStorage(std::string root) : root(std::move(root)), ans(0, "", nullptr, VET_Var)
{

}
VarStorage::~VarStorage()
{

}

std::string VarStorage::GetVariablePath(const std::string& name)
{

}

const VarStorageEntry* VarStorage::GetEntry(VarStorageKey key) const noexcept
{
    auto iter = std::find(this->entries.begin(), this->entries.end(), [key] (VarStorageEntry* item) -> bool
    {
        return item->key == key;
    });

    return iter == this->entries.end() ? nullptr : iter;
}
VarStorageEntry* VarStorage::GetEntry(VarStorageKey key) noexcept
{
    return const_cast<VarStorageEntry*>(const_cast<const VarStorage*>(this)->GetEntry(key));
}