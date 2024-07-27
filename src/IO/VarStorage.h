//
// Created by exdisj on 5/26/24.
//

#ifndef JASON_VARSTORAGE_H
#define JASON_VARSTORAGE_H

#include <string>

#include "../Calc/VariableType.h"

using VarStorageKey = unsigned;

class VarStorageEntry
{
private:
    VarStorageKey key;
    std::string name;
    VariableType* data;
    bool write_to_file = false;

    VarStorageEntry(VarStorageKey key, const std::string& name, VariableType* data, bool write_to_files = true);

    bool& IsWritingToFiles() noexcept;

    bool SaveToPath(const std::string& path) const noexcept;
    bool LoadFromPath(const std::string& path) noexcept;

    void SetData(VariableType* New);

public:
    VarStorageEntry(const VarStorageEntry& obj) = delete;
    VarStorageEntry(VarStorageEntry&& obj) = delete;
    ~VarStorageEntry();

    VarStorageEntry& operator=(const VarStorageEntry& obj) = delete;
    VarStorageEntry& operator=(VarStorageEntry& Obj) = delete;

    [[nodiscard]] bool HasData() const noexcept;
    [[nodiscard]] const VariableType& Data() const;
    VarStorageKey Key() const noexcept;
    const std::string& Name() const noexcept;

    void Print(std::ostream& out) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const VarStorageEntry& obj) noexcept;

class VarStorage
{
private:

public:
};


#endif //JASON_VARSTORAGE_H
