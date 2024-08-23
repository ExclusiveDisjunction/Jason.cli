//
// Created by exdisj on 8/13/24.
//

#ifndef JASON_PACKAGEHANDLE_H
#define JASON_PACKAGEHANDLE_H

#include <filesystem>
#include <fstream>

struct FileHandle
{
public:
    explicit FileHandle(std::filesystem::path path, std::ios::openmode flags = (std::ios::out | std::ios::in));
    FileHandle(const FileHandle& obj) = delete;
    FileHandle(FileHandle&& obj) noexcept;
    ~FileHandle();

    void Close() noexcept;

    std::filesystem::path path;
    std::fstream file;
};

#endif //JASON_PACKAGEHANDLE_H
