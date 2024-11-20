#include "PackageEntryIndex.h"

PackageEntryIndex::PackageEntryIndex(PackageEntryKey key, PackageEntryType type, std::string name, unsigned char state, VariableTypes data_type, std::vector<unsigned int> pages) noexcept : key(key), type(type), name(std::move(name)), state(state), data_type(data_type), pages(std::move(pages))
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
bool PackageEntryIndex::IsModified() const noexcept
{
    return this->state & modified;
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
    this->state |= modified;
}
void PackageEntryIndex::IsReadOnly(bool New) noexcept
{
    this->state |= (New ? readonly : 0);
    this->state |= modified;
}
void PackageEntryIndex::IsModified(bool New) noexcept
{
    if (static_cast<bool>(this->state & modified) == New) //No change
        return;

    this->state &= ~modified;
    this->state |= (New ? modified : 0);
}
void PackageEntryIndex::Name(const std::string& New) noexcept
{
    this->name = New;
    this->state |= modified;
}

std::ostream& operator<<(std::ostream& out, const PackageEntryIndex& obj) noexcept
{
    // ID NAME TYPE DATA_TYPE LOAD_IMM(0, 1) READ_ONLY(0, 1) PAGE_LEN PAGES...
    out << obj.key.EntryID << ' ' 
        << obj.name << ' '
        << (obj.type == Variable ? "var" : obj.type == Environment ? "env" : "tmp") << ' ' 
        << obj.data_type << ' '
        << (bool)(obj.state & PackageEntryIndex::load_imm) << ' ' 
        << (bool)(obj.state & PackageEntryIndex::readonly) << ' '
        << obj.pages.size() << ' ';
    for (auto& page : obj.pages)
        out << page << ' ';
        
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

    //Get pages
    obj.pages.resize(pageLen, 0);
    for (auto& page : obj.pages) 
    {
        if (!in)
            throw std::logic_error("The stream was exhausted before the pages could be completeley read");

        in >> page;
    }

    return in;
}