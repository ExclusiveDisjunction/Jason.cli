//
// Created by Hollan on 7/27/24.
//

#include "VarStorage.h"

#include <string>
#include <fstream>

VarStorageEntry::VarStorageEntry(VarStorageKey key, std::string name, VariableType* data, VarEntryType type, bool write_to_files) :
key(key), name(std::move(name)), data(data), type(type), write_to_file(write_to_files)
{
    if (type != VET_Temp && name.empty())
        throw std::logic_error("Name cannot be empty if the type is not a temporary.");
}
VarStorageEntry::~VarStorageEntry()
{
    SetData(nullptr);
}

bool& VarStorageEntry::IsWritingToFiles() noexcept
{
    return this->write_to_file;
}

bool VarStorageEntry::SaveToPath(const std::string& path) const noexcept
{
    if (this->type == VET_Temp) //Temporaries are not written to files.
        return true;
    else if (!this->write_to_file)
        return false;

    std::ofstream file(path, std::ios::trunc);
    if (!file)
        return false;

    if (!this->data)
        file << "NAN";
    else
        this->data->Sterilize(file);

    file.close();
    return true;
}
bool VarStorageEntry::LoadFromPath(const std::string& path) noexcept
{
    std::ifstream file(path);
    if (!file)
        return false;

    this->data = VariableType::FromSterilized(file);
    return this->data != nullptr;
}

void VarStorageEntry::SetData(VariableType* New)
{
    delete this->data;
    this->data = New;
}

[[nodiscard]] bool VarStorageEntry::HasData() const noexcept
{
    return this->data != nullptr;
}
[[nodiscard]] const VariableType& VarStorageEntry::Data() const
{
    if (!this->data)
        throw std::logic_error("No data!");

    return *this->data;
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
    out << "(" << name << " : ";
    if (this->data)
        this->data->Print(out);
    else
        out << " (None)";

    out << " )";
}