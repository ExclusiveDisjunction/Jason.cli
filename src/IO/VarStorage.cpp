//
// Created by exdisj on 5/26/24.
//

#include "VarStorage.h"

#include <utility>
#include <algorithm>
#include <filesystem>

std::string GetVETPrefix(const VarEntryType& t) noexcept
{
    switch (t)
    {
        case VET_Var:
            return "v.";
        case VET_Env:
            return "e.";
        case VET_Temp:
            return "t.";
    }
}

VarStorage::VarStorage(std::string root) : ans(0, "", nullptr, VET_Var), save_to_files(true)
{
    if (!std::filesystem::exists(root) || !std::filesystem::is_directory(root))
        throw std::logic_error("Root directory does not exist, or the path provided does not point to a directory.");

    if (!root.ends_with('/') || !root.ends_with('\\')) //We need that trailing slash, and we include both because windows :(
        root += '/';

    this->root = std::move(root);
}
VarStorage::~VarStorage() = default;

const VarStorageEntry* VarStorage::GetEntry(VarStorageKey key) const noexcept
{
    auto iter = std::find_if(this->entries.begin(), this->entries.end(), [key] (const VarStorageEntry& item) -> bool
    {
        return item.key == key;
    });

    return iter == this->entries.end() ? nullptr : &(*iter);
}
VarStorageEntry* VarStorage::GetEntry(VarStorageKey key) noexcept
{
    return const_cast<VarStorageEntry*>(const_cast<const VarStorage*>(this)->GetEntry(key));
}

[[nodiscard]] bool VarStorage::IsSavingToFiles() const noexcept
{
    return this->save_to_files;
}
void VarStorage::SetSavingToFiles(bool New) noexcept
{
    if (this->save_to_files != New)
    {
        this->save_to_files = New;

        for (auto& ent : this->entries)
            ent.write_to_file = New;
    }
}

[[nodiscard]] bool VarStorage::IndexVariables() noexcept
{

}
void VarStorage::ReIndexKeys() noexcept
{

}

[[nodiscard]] std::optional<VarStorageKey> VarStorage::GetEntryKey(const std::string& name, VarEntryType type) noexcept
{

}

[[nodiscard]] bool VarStorage::DoesEntryExist(VarStorageKey key) const noexcept
{

}
[[nodiscard]] const VariableType& VarStorage::GetEntryValue(VarStorageKey key) const
{

}
[[nodiscard]] VariableType& VarStorage::GetEntryValue(VarStorageKey key)
{

}
[[nodiscard]] bool VarStorage::SetEntryValue(VarStorageKey key) noexcept
{

}

[[nodiscard]] bool VarStorage::LoadEntry(VarStorageKey key) noexcept //Loads a variable from a stored file, if it is stored there
{

}
[[nodiscard]] bool VarStorage::ResetEntry(VarStorageKey key) noexcept//Clears the value from a specified entry
{

}

[[nodiscard]] bool VarStorage::PopEntry(VarStorageEntry key) noexcept //Remove from current instance, but leaves it in file system
{

}
[[nodiscard]] bool VarStorage::DropEntry(VarStorageEntry key) noexcept //Pop and remove from file system
{

}

[[nodiscard]] std::optional<VarStorageKey> VarStorage::AddEntry(const std::string& name, VarEntryType type, VariableType* data) noexcept
{

}