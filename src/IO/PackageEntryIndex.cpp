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
    return out << obj.key.EntryID << ' ' << (obj.type == Variable ? "var" : obj.)
}
std::istream& operator>>(std::istream& in, PackageEntryIndex& obj)
{

}