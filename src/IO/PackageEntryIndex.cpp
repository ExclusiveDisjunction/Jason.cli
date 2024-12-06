#include "PackageEntryIndex.h"

PackageEntryIndex::PackageEntryIndex(PackageEntryKey key, PackageEntryType type, std::string name, unsigned char state, VariableTypes data_type) noexcept : key(key), type(type), name(std::move(name)), state(state), data_type(data_type)
{

}
PackageEntryIndex::PackageEntryIndex(std::istream& in)
{
    in >> *this;
}

bool PackageEntryIndex::LoadImmediate() const noexcept
{
    return this->state & load_imm;
}
bool PackageEntryIndex::IsReadOnly() const noexcept
{
    return this->state & readonly;
}
bool PackageEntryIndex::IsTemporary() const noexcept
{
    return this->type == Temporary;
}
const std::string& PackageEntryIndex::Name() const noexcept
{
    return this->name;
}
const PackageEntryKey& PackageEntryIndex::Key() const noexcept
{
    return this->key;
}

void PackageEntryIndex::LoadImmediate(bool New) noexcept
{
    this->state |= (New ? load_imm : 0);
}
void PackageEntryIndex::IsReadOnly(bool New) noexcept
{
    this->state |= (New ? readonly : 0);
}
void PackageEntryIndex::Name(const std::string& New) noexcept
{
    this->name = New;
}

std::ostream& operator<<(std::ostream& out, const PackageEntryIndex& obj) noexcept
{
    // ID NAME TYPE DATA_TYPE LOAD_IMM(0, 1) READ_ONLY(0, 1) PAGE_LEN PAGES...
    out << obj.key.EntryID << ' ' 
        << obj.name << ' '
        << (obj.type == Variable ? "var" : obj.type == Environment ? "env" : "tmp") << ' ' 
        << obj.data_type << ' '
        << (bool)(obj.state & PackageEntryIndex::load_imm) << ' ' 
        << (bool)(obj.state & PackageEntryIndex::readonly) << ' ';
        
    return out;
}
std::istream& operator>>(std::istream& in, PackageEntryIndex& obj)
{
    // ID NAME TYPE LOAD_IMM(0, 1) READ_ONLY(0, 1) PAGE_LEN PAGES...

    obj = PackageEntryIndex(); //Reset our index

    std::string typeRaw;
    bool load = false, read = false;
    unsigned pageLen = 0;
    in >> obj.key.EntryID >> obj.name >> typeRaw >> obj.data_type >>  load >> read >> pageLen;

    //Parse type
    if (typeRaw == "var")
        obj.type = Variable;
    else if (typeRaw == "env")
        obj.type = Environment;
    else if (typeRaw == "tmp")
        obj.type = Temporary;
    else 
        throw std::logic_error("Could not resolve variable entry type '" + typeRaw + '\'');

    //Parse state
    obj.state |= load ? PackageEntryIndex::load_imm : 0;
    obj.state |= read ? PackageEntryIndex::readonly : 0;

    return in;
}