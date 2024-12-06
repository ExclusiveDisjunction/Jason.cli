//
// Created by exdisj on 8/9/24.
//

#include "FileHandle.h"
#include <cstring>
#include <sstream>

FileHandle::FileHandle(std::filesystem::path path, std::ios::openmode flags) : FileHandle()
{
    Open(std::move(path), flags);
}
FileHandle::FileHandle(FileHandle&& obj) noexcept : path(std::move(obj.path)), file(std::move(obj.file))
{

}
FileHandle::~FileHandle()
{
    Close();
}

void FileHandle::Open(std::filesystem::path path , std::ios::openmode flags)
{
    Close();

    this->path = std::move(path);
    file.open(this->path, flags);

    if (!this->file)
    {
        if (this->file.bad())
            throw std::logic_error("Could not open file at that path. (badbit set)");
        else if (this->file.fail())
        {
            std::stringstream ss;
            ss << "Could not open file at that path because of '" << strerror(errno) << "'.";
            throw std::logic_error(ss.str());
        }
        else
            throw std::logic_error("Could not open file at that path.");
    }

    this->file.seekg(0, std::ios::beg);
}
void FileHandle::ReOpen(std::ios::openmode flags)
{
    std::filesystem::path this_path = this->path;
    Close();

    Open(this_path, flags);
}

FileHandle& FileHandle::operator=(FileHandle&& obj) noexcept
{
    Close();

    this->file = std::move(obj.file);
    this->path = std::move(obj.path);
    return *this;
}


void FileHandle::Close() noexcept
{
    this->file.close();
    this->path.clear();
}