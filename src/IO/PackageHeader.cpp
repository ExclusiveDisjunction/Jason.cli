//
// Created by Hollan on 8/21/24.
//

#include "PackageHeader.h"

#include <sstream>
#include <algorithm>

PackageHeader::PackageHeader(FileHandle&& handle) : handle(std::move(handle)), version(JASON_CURRENT_VERSION), author(), readonly(false), modified(false), page_size(4)
{

}

const std::filesystem::path& PackageHeader::GetLocation() const noexcept
{
    return handle.path;
}

bool PackageHeader::Write() noexcept
{
    this->handle.file.close();
    this->handle.file.open(this->handle.path, std::ios::in | std::ios::out | std::ios::trunc);

    this->handle.file << *this;
    this->handle.file.flush();

    modified = false;
    return this->handle.file.good();
}
bool PackageHeader::WriteIfModified() noexcept
{
    if (modified)
        return Write();
    else
        return true;
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
void PackageHeader::SetVersion(Version New) noexcept
{   
    SetValue(version, New);
}

const std::optional<std::string>& PackageHeader::GetAuthor() const noexcept
{
    return this->author;
}
void PackageHeader::SetAuthor(std::optional<std::string> New) noexcept
{
    SetValue(author, New);
}

bool PackageHeader::IsReadOnly() const noexcept
{
    return this->readonly;
}
void PackageHeader::SetReadOnly(bool New) noexcept
{
    SetValue(readonly, New);
}

unsigned PackageHeader::PageSize() const noexcept
{
    return this->page_size;
}
void PackageHeader::PageSize(unsigned New) noexcept
{
    SetValue(this->page_size, New);
}

void PackageHeader::Close()
{
    this->handle.Close();
}

std::ostream& operator<<(std::ostream& out, const PackageHeader& obj)
{
    out << "version=" << obj.version << '\n' <<
           (obj.author.has_value() ? "author=" + *obj.author + '\n' : std::string()) << 
           "readonly=" << (obj.readonly ? 't' : 'f') << '\n' <<
           "pagesize=" << obj.page_size;

    return out;
}
std::istream& operator>>(std::istream& in, PackageHeader& obj)
{
    obj.modified = false;
    obj.version = Version();
    obj.author = {};
    obj.readonly = false;
    obj.page_size = 4;

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
        else if (name == "pagesize")
        {
            std::stringstream num_str(value);
            num_str >> obj.page_size;
        }
    }
    return in;
}
