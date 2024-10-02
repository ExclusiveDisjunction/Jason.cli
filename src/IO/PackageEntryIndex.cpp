#include "PackageEntryIndex.h"

PackageEntryIndex::PackageEntryIndex(PackageEntryKey key, PackageEntryType type, std::string name, unsigned char state) noexcept : key(key), type(type), name(std::move(name)), state(state)
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
void PackageEntryIndex::IsModified(bool New) noexcept
{
    this->state |= (New ? modified : 0);
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
void PackageEntryIndex::Name(const std::string& New) noexcept
{
    this->name = New;
    this->state |= modified;
}

std::ostream& operator<<(std::ostream& out, const PackageEntryIndex& obj) noexcept
{
    out << obj.key.EntryID << ' ' << (obj.type == Variable ? "var" : obj.type == Environment ? "env" : "tmp") << " f:";
    if (obj.state & PackageEntryIndex::load_imm)
        out << '!';
    if (obj.state & PackageEntryIndex::readonly)
        out << '~';

    out << ' ' << obj.name;
    return out;
}
std::istream& operator>>(std::istream& in, PackageEntryIndex& obj)
{
    std::string type, flags;
    in >> obj.key.EntryID >> type >> flags >> obj.name;

    //Parse type
    if (type == "var")
        obj.type = Variable;
    else if (type == "env")
        obj.type = Environment;
    else if (type == "tmp")
        obj.type = Temporary;
    else 
        throw std::logic_error("Could not resolve variable entry type '" + type + '\'');

    //Parse state
    obj.state = 0;
    if (flags.starts_with("f:"))
    {
        for (const char& item : flags.substr(2))
        {
            switch (item)
            {
                case '!': //Load imm
                    obj.state |= PackageEntryIndex::load_imm;
                    break;
                case '~': //Readonly
                    obj.state |= PackageEntryIndex::readonly;
                    break;
                default:
                    throw std::logic_error(std::string("Could not resolve flag '") + item + "\'");
            }
        }
    }
    else
        throw std::logic_error("Flag specifier format is incorrect.");

    return in;
}