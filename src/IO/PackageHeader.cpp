//
// Created by Hollan on 8/21/24.
//

#include "PackageHeader.h"

std::optional<PackageHeader> PackageHeader::FromFile(std::istream& in, std::optional<std::streamoff> endLoc)
{

}
PackageHeader::PackageHeader(std::optional<FileHandle>&& handle, Version ver, std::optional<std::string> author, bool locked) : handle(std::move(handle)), ver(ver), author(std::move(author)), locked(locked)
{

}

void PackageHeader::Output(std::ostream& out) const noexcept
{
    out << "version=" << this->ver << ' ' << (author.has_value() ? "author=" + *author + " " : std::string()) << "locked=" << (locked ? 't' : 'f');
}

std::ostream& operator<<(std::ostream& out, const Version& obj) noexcept
{
    out << obj.Major << '.' << obj.Minor << '.' << obj.Release;
    return out;
}
std::istream& operator>>(std::istream& in, Version& obj)
{
    char p; //Placeholder for the '.'. This way, the istream will split up the reading between the three parts. 
    in >> obj.Major >> p >> obj.Minor >> p >> obj.Release;
    return in;
}
