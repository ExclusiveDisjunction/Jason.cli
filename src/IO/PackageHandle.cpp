//
// Created by exdisj on 8/9/24.
//

#include "PackageUtility.h"

PackageHandle::PackageHandle(std::string path, std::ios::openmode flags) : path(std::move(path)), file()
{
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
PackageHandle::PackageHandle(PackageHandle&& obj) noexcept : path(std::move(obj.path)), file(std::move(obj.file))
{

}
PackageHandle::~PackageHandle()
{
    Close();
}

void PackageHandle::Close() noexcept
{
    this->file.close();
    this->path.clear();
}
