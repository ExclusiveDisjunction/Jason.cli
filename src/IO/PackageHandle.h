//
// Created by exdisj on 8/13/24.
//

#ifndef JASON_PACKAGEHANDLE_H
#define JASON_PACKAGEHANDLE_H

#include <filesystem>
#include <fstream>

struct PackageHandle
{
public:
    explicit PackageHandle(std::filesystem::path path, std::ios::openmode flags = (std::ios::out | std::ios::in));
    PackageHandle(const PackageHandle& obj) = delete;
    PackageHandle(PackageHandle&& obj) noexcept;
    ~PackageHandle();

    void Close() noexcept;

    std::filesystem::path path;
    std::fstream file;
};

#endif //JASON_PACKAGEHANDLE_H
