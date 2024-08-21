//
// Created by exdisj on 8/7/24.
//

#include "PackageEntry.h"
#include "../Common.h"

#include <utility>

PackageEntry::PackageEntry(const std::string& line, std::ostream& output, PackageEntryKey key) : key(key), type(Variable), data(nullptr), load_imm(false)
{
    std::stringstream in(line);
    if (!ReadFromFile(in, output))
        throw std::logic_error("Could not index entry.");
}
PackageEntry::PackageEntry(std::istream& in, std::ostream& output, PackageEntryKey key) : key(key), type(Variable), data(nullptr), load_imm(false)
{
    if (!ReadFromFile(in, output))
        throw std::logic_error("Could not index entry.");
}
PackageEntry::PackageEntry(PackageEntryKey key, std::string name, VariableType* data, PackageEntryType type, bool load_imm) : key(key), name(std::move(name)), data(data), type(type), load_imm(load_imm)
{
    if (this->type != PackageEntryType::Temporary && name.empty())
        throw std::logic_error("Cannot construct a variable entry with no name, unless type is temporary.");

    if (this->type == PackageEntryType::Temporary)
        this->name.clear();
}
PackageEntry::PackageEntry(PackageEntry&& obj) noexcept : key(obj.key), type(obj.type), name(std::move(obj.name)), data(std::exchange(obj.data, nullptr)), load_imm(obj.load_imm)
{

}
PackageEntry::~PackageEntry()
{
    delete data;
    data = nullptr;
}

[[nodiscard]] bool PackageEntry::WriteCore(std::ostream& out) const noexcept
{
    if (!out)
        return false;

    switch (this->type)
    {
        case PackageEntryType::Variable:
            out << "var";
            break;
        case PackageEntryType::Environment:
            out << "env";
            break;
        case PackageEntryType::Temporary:
        default:
            return false;
    }

    out << " " << this->name << (this->load_imm ? '!' : '\0') << " ";
    return true;
}
[[nodiscard]] bool PackageEntry::WriteFromMemory(std::ostream& out) const noexcept
{
    if (!out || !this->data || !WriteCore(out))
        return false;

    return WriteData(out);
}
[[nodiscard]] bool PackageEntry::WriteFromInflated(std::ostream& out, std::istream& sterilized) const noexcept
{
    if (!out || !sterilized || !WriteCore(out))
        return false;

    out << sterilized.rdbuf();
    return true;
}
[[nodiscard]] bool PackageEntry::WriteData(std::ostream& out) const noexcept
{
    if (!out || !this->data)
        return false;

    this->data->Sterilize(out);
    return out.good();
}
[[nodiscard]] bool PackageEntry::WriteSchematic(std::ostream& out) const noexcept
{
    if (!out)
        return false;

    out << this->key.EntryID << ' ' << (this->type == Variable ? "var" : this->type == Environment ? "env" : "tmp") << ' ' << this->name << ' ' << (this->load_imm ? "i" : "n");
    return true;
}
[[nodiscard]] bool PackageEntry::ReadFromFile(std::istream& in, std::ostream& sterilizedOut) noexcept
{
    if (!in)
        return false;

    this->type = Variable;
    this->load_imm = false;
    this->name.clear();
    this->Data(nullptr);

    std::string type_r, load;
    in >> type_r;
    if (type_r == "var")
        this->type = Variable;
    else if (type_r == "env")
        this->type = Environment;
    else
        return false;

    in >> this->name;

    std::streamoff pos = in.tellg();
    in >> load;
    if (load == "!")
        this->load_imm = true;
    else
        in.seekg(pos); //We shouldn't read the value at "load", so we backtrack.

    if (this->load_imm) //We treat the rest of the data as a loaded value.
    {
        VariableType* result = VariableType::FromSterilized(in);
        if (!result)
            return false;
        else
            this->Data(result);
    }

    std::string sterilized;
    std::getline(in, sterilized);
    trim(sterilized);
    sterilizedOut << sterilized;

    return true;
}

[[nodiscard]] const VariableType& PackageEntry::Data() const
{
    if (!this->data)
        throw std::logic_error("Entry is empty, cannot dereference.");

    return *this->data;
}
void PackageEntry::Data(VariableType* New) noexcept
{
    delete data;
    this->data = New;
}

[[nodiscard]] bool PackageEntry::HasData() const noexcept
{
    return this->data != nullptr;
}
[[nodiscard]] PackageEntryKey PackageEntry::Key() const noexcept
{
    return this->key;
}
[[nodiscard]] const std::string& PackageEntry::Name() const noexcept
{
    return this->name;
}
[[nodiscard]] bool PackageEntry::LoadImm() const noexcept
{
    return this->load_imm;
}
[[nodiscard]] PackageEntryType PackageEntry::GetType() const noexcept
{
    return this->type;
}
[[nodiscard]] bool PackageEntry::IsTemporary() const noexcept
{
    return this->type == Temporary;
}
[[nodiscard]] std::filesystem::path PackageEntry::GetPath(const std::filesystem::path& source) const noexcept
{
    return source / std::to_string(this->key.EntryID);
}




