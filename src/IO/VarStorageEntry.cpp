//
// Created by Hollan on 7/27/24.
//

#include "VarStorage.h"

VarStorageEntry::VarStorageEntry(VarStorageKey key, const std::string& name, VariableType* data, bool write_to_files) :
key(key), name(name), data(data), write_to_file(write_to_files)
{

}
VarStorageEntry::~VarStorageEntry()
{
    if (data)
        delete data;
    data = nullptr;
}

bool& VarStorageEntry::IsWritingToFiles() noexcept
{
    return this->write_to_file;
}

bool VarStorageEntry::SaveToPath(const std::string& path) const noexcept
{

}
bool VarStorageEntry::LoadFromPath(const std::string& path) noexcept
{

}

void SetData(VariableType* New)
{
    if (this->Data)
        delete this->Data;

    this->Data = New;
}

[[nodiscard]] bool VarStorageEntry::HasData() const noexcept
{
    return this->Data != nullptr;
}
[[nodiscard]] const VariableType& VarStorageEntry::Data() const
{
    if (!this->Data)
        throw std::logic_error("No data!");

    return *this->Data;
}
VarStorageKey VarStorageEntry::Key() const noexcept
{
    return this->key;
}
const std::string& VarStorageEntry::Name() const noexcept
{
    return this->name;
}

void VarStorageEntry::Print(std::ostream& out) const noexcept
{
    out << "(" << name << ' : ';
    if (this->Data)
        this->data->Print(out);
    else
        out << " (None)";

    out << " )";
}