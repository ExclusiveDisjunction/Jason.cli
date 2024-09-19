//
// Created by exdisj on 8/7/24.
//

#include "PackageEntry.h"
#include "Package.h"
#include "../Common.h"

#include "PackageIndex.h"

#include <utility>
#include <string>
#include <fstream>
#include <sstream>

PackageEntry::PackageEntry(PackageEntryKey key, std::string name, VariableType* data, PackageEntryType type, Package* parent, unsigned char state) : key(key), name(std::move(name)), data(nullptr), type(type), parent(parent), state(state)
{
    if (this->type != PackageEntryType::Temporary && name.empty())
        throw std::logic_error("Cannot construct a variable entry with no name, unless type is temporary.");

    if (this->type == PackageEntryType::Temporary)
        this->name.clear();

    this->Data(data);
}
PackageEntry::~PackageEntry()
{
    if (this->parent)
        this->parent->ReleaseEntry(this->key.EntryID);

    (void)Unload();
}

PackageEntry* PackageEntry::FromIndexTableLine(std::istream& in, Package* parent)
{
    if (!in)
        return nullptr;

    PackageEntry* result = new PackageEntry();
    
    result->parent = parent;
    result->key.PackageID = !parent ? 0 : parent->GetID();
    ReadIndex(in, *result);

    return result;
}
PackageEntry* PackageEntry::FromIndexTableLine(const std::string& line, Package* parent)
{
    std::stringstream ss(line);
    return FromIndexTableLine(ss, parent);
}
PackageEntry* PackageEntry::ExpandFromCompressed(std::istream& in, Package* parent, std::ostream& out)
{
    PackageEntry result;
    if (!in)
        return nullptr;

    //Process header & fill structure
    result.parent = parent;
    result.key.PackageID = !parent ? 0 : parent->GetID();
    ReadIndex(in, result);

    //Output to the specified output location.
    std::string sterilized;
    std::getline(in, sterilized);
    
    std::stringstream ss_steril(sterilized);
    sterilized.clear();

    out << ss_steril.rdbuf();
    ss_steril.seekp(0, std::ios::beg);

    //Load from sterilized string, prevents opening & closing of file streams.
    if (result.state & load_imm)
    {
        if (!result.Load(ss_steril))
            throw std::logic_error("The load idmediatley flag was set, but the file could not be resolved, or it does not contain proper formatted data");
    }
}
PackageEntry* PackageEntry::ExpandFromCompressed(const std::string& line, Package* parent, std::ostream& out)
{
    std::stringstream ss(line);
    return ExpandFromCompressed(ss, parent, out);
}

bool PackageEntry::WriteData(std::ostream& out) const noexcept
{
    if (!out || !this->data)
        return false;

    if (*this->data)
        (*this->data)->Sterilize(out);
    else
        out << "NULL";

    return out.good();
}
bool PackageEntry::WriteData() const noexcept
{
    std::filesystem::path thisPath = this->GetPath();
    std::ofstream out(thisPath, std::ios::trunc);

    return WriteData(out);
}
bool PackageEntry::WriteIndex(std::ostream& out) const noexcept
{
    if (!out)
        return false;

    out << this->key.EntryID << ' ' << (this->type == Variable ? "var" : this->type == Environment ? "env" : "tmp") << " f:" << (this->state & load_imm ? '!' : 0) << (this->state & readonly ? '~' : 0) << ' ' << this->name;
    return true;
}
void PackageEntry::ReadIndex(std::istream& in, PackageEntry& result) 
{
    if (!in)
        throw std::logic_error("Bad stream");

    std::string type, flags;
    in >> result.key.EntryID >> type >> flags >> result.name;

    //Parse type
    if (type == "var")
        result.type = Variable;
    else if (type == "env")
        result.type = Environment;
    else if (type == "tmp")
        result.type = Temporary;
    else 
        throw std::logic_error("Could not resolve variable entry type '" + type + '\'');

    //Parse state
    result.state = 0;
    for (const char& item : flags) 
    {
        switch (item) 
        {
            case '!': //Load imm 
                result.state |= load_imm;
                break;
            case '~': //Readonly
                result.state |= readonly;
                break;
            default:
                throw std::logic_error("Could not resolve flag '" + item + '\'');
        }
    }
}

bool PackageEntry::Load() noexcept
{
    if (this->data)
        return true;

    std::filesystem::path thisPath = this->GetPath();
    std::ifstream in(thisPath);
    if (!in) //Could not load at path
    {
        in.close();
        std::ofstream out(thisPath, std::ios::trunc);
        if (!out) //Failed to construct the path
        {
            std::cerr << "When loading package entry '" << (this->parent ? this->parent->GetName() : "") << "::" << this->name << "', the located path could not be found." << std::endl;
            return false;
        }  
        
        out << "NULL";
        out.close();
        this->data = nullptr;
        return true;
    }
    else
        return Load(in);
}
bool PackageEntry::Load(std::istream& in) noexcept 
{
    if (!in)
        return false;

    in.seekg(0, std::ios::beg);
    this->data = VariableType::FromSterilized(in);
    return this->data != nullptr;
}
bool PackageEntry::Unload() noexcept
{
    delete this->data.value_or(nullptr);
    this->data = {};
    return true;
}
bool PackageEntry::Reset() noexcept
{
    if (this->parent)
    {
        std::filesystem::path thisPath = this->GetPath();
        std::ofstream out(thisPath, std::ios::trunc);
        if (!out)
            return false;
    }

    return Unload();
}

const VariableType& PackageEntry::Data() const
{
    if (!this->data.has_value())
        throw std::logic_error("The data contained is not loaded.");
    else if (! *(this->data))
        throw std::logic_error("This package entry contains no data.");

    return *(*this->data);
}
void PackageEntry::Data(VariableType* New) noexcept
{
    Reset();
    this->data = New;
    (void)WriteData();
}

std::optional<bool> PackageEntry::HasData() const noexcept
{
    return !this->data ? std::optional<bool>() : *this->data != nullptr;
}

bool PackageEntry::LoadImmediate() const noexcept
{
    return this->state & load_imm;
}
bool PackageEntry::IsReadOnly() const noexcept
{
    return this->state & readonly;
}
bool PackageEntry::IsTemporary() const noexcept
{
    return this->type == Temporary;
}

PackageEntryKey PackageEntry::Key() const noexcept
{
    return this->key;
}
PackageEntryType PackageEntry::Type() const noexcept
{
    return this->type;
}
std::filesystem::path PackageEntry::GetPath() const
{
    if (!this->parent)
        throw std::logic_error("Could not locate parent's path.");

    return this->parent->VarLocation() / std::to_string(this->key.EntryID);
}
const std::string& PackageEntry::Name() const noexcept
{
    return this->name;
}

std::ostream& operator<<(std::ostream& out, const PackageEntry& obj) noexcept
{
    (void)obj.WriteData(out);
    return out;
}