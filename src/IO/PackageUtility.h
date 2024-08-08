//
// Created by exdisj on 8/8/24.
//

#ifndef JASON_PACKAGEUTILITY_H
#define JASON_PACKAGEUTILITY_H

#include <string>
#include <fstream>

struct PackageHandle
{
public:
    explicit PackageHandle(std::string path) : path(std::move(path)), file()
    {
        this->file.open(path, (std::ios::app | std::ios::in | std::ios::out));
        if (!this->file)
            throw std::logic_error("The file at that path could not be opened.");

        this->file.seekg(0, std::ios::beg);
    }
    PackageHandle(std::fstream file, std::string path) : path(std::move(path)), file(std::move(file))
    {
        if (!this->file)
            throw std::logic_error("The input stream provided is invalid.");

        this->file.seekg(0, std::ios::beg);
    }
    PackageHandle(const PackageHandle& obj) = delete;
    PackageHandle(PackageHandle&& obj) noexcept : path(std::move(obj.path)), file(std::move(obj.file))
    {

    }
    ~PackageHandle()
    {
        file.close();
    }


    std::string path;
    std::fstream file;
};

struct PreProcessedPackage
{
public:
    PreProcessedPackage(PackageHandle& handle, bool toLoad, bool processed = false) : handle(std::move(handle)), processed(processed), toLoad(toLoad) {}

    PackageHandle handle;
    bool processed, toLoad;
};

class IndexedEntry
{
public:
    IndexedEntry() : beg(std::streampos()), end(std::streampos()), name(std::string()), type(PackageEntryType::Temporary), load_imm(false) {}
    IndexedEntry(std::streampos beg, std::streampos end, std::string name, PackageEntryType type, bool load_imm) : beg(beg), end(end), name(std::move(name)), type(type), load_imm(load_imm) {}

    std::streampos beg, end;
    std::string name;
    PackageEntryType type;
    bool load_imm;
};
class IndexedPackage
{
public:
    PackageHandle handle;
    std::vector<IndexedEntry> entries;
};

class UnloadedPackage
{
public:
    UnloadedPackage(PackageHandle& handle, unsigned long PackageID) : handle(std::move(handle)), ID(PackageID) {}

    PackageHandle handle;
    unsigned long ID;
};

class Version
{
public:
    Version(unsigned Major, unsigned Minor, unsigned Release) : Major(Major), Minor(Minor), Release(Release) {}

    unsigned Major;
    unsigned Minor;
    unsigned Release;
};
std::ostream& operator<<(std::ostream& out, const Version& obj) noexcept;
std::istream& operator>>(std::istream& in, Version& obj);

#endif //JASON_PACKAGEUTILITY_H
