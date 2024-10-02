//
// Created by Hollan on 8/21/24.
//

#include "PackageHeader.h"

#include <sstream>

PackageHeader::PackageHeader(FileHandle&& handle, Version ver, std::optional<std::string>&& author, bool readonly) : handle(std::move(handle)), version(ver), author(std::move(author)), readonly(readonly)
{
    if (!this->handle.file)
        throw std::logic_error("File provided is invalid");
}

const std::filesystem::path& PackageHeader::GetLocation() const noexcept
{
    return handle.path;
}

bool PackageHeader::Write() noexcept
{
    this->handle.file << *this;
    this->handle.file.flush();
    return this->handle.file.good();
}
bool PackageHeader::Read() noexcept
{
    this->handle.file >> *this;
    return !this->handle.file.bad();
}

const Version& PackageHeader::GetVersion() const noexcept
{
    return this->version;
}
bool PackageHeader::SetVersion(Version New) noexcept
{   
    return SetValue(version, New);
}

const std::optional<std::string>& PackageHeader::GetAuthor() const noexcept
{
    return this->author;
}
bool PackageHeader::SetAuthor(std::optional<std::string> New) noexcept
{
    return SetValue(author, New);
}

bool PackageHeader::IsReadOnly() const noexcept
{
    return this->readonly;
}
bool PackageHeader::SetReadOnly(bool New) noexcept
{
    return SetValue(readonly, New);
}

void PackageHeader::Close()
{
    this->handle.Close();
}

std::ostream& operator<<(std::ostream& out, const PackageHeader& obj)
{
    out << "version=" << obj.version << '\n' <<
           (obj.author.has_value() ? "author=" + *obj.author + '\n' : std::string()) << 
           "readonly=" << (obj.readonly ? 't' : 'f');

    return out;
}
std::istream& operator>>(std::istream& in, PackageHeader& obj)
{
    obj.version = Version();
    obj.author = {};
    obj.readonly = false;

    std::string line;
    while (!in.eof())
    {
        std::getline(in, line);
        if (line.empty())
            continue;
        
        auto iter = std::find(line.begin(), line.end(), '=');
        if (iter == line.end())
            continue;
        
        std::string name = line.substr(0, iter - line.begin());
        std::string value = line.substr(iter - line.begin() + 1);
        if (name == "version")
        {
            std::stringstream ss(value);
            ss >> obj.version;
        }
        else if (name == "author")
            obj.author = value;
        else if (name == "readonly")
            obj.readonly = value == "t";
    }
    return in;
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

bool Version::operator==(const Version& obj) const noexcept
{
    return this->Major == obj.Major && this->Minor == obj.Minor && this->Release == obj.Release;
}
bool Version::operator!=(const Version& obj) const noexcept
{  
    return this->Major != obj.Major || this->Minor != obj.Minor || this->Release != obj.Release;
}
