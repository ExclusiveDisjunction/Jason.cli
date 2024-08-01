//
// Created by exdisj on 5/26/24.
//

#ifndef JASON_VAR_STORAGE_H
#define JASON_VAR_STORAGE_H

#include <string>
#include <vector>
#include <optional>

#include "../Calc/VariableType.h"

using VarStorageKey = unsigned;

enum VarEntryType
{
    VET_Var,
    VET_Temp,
    VET_Env
};

class VarStorageEntry
{
private:
    VarStorageKey key;
    VarEntryType type;
    std::string name;
    VariableType* data;
    bool write_to_file = false;

    VarStorageEntry(VarStorageKey key, std::string name, VariableType* data, VarEntryType type, bool write_to_files = true);

    bool& IsWritingToFiles() noexcept;

    [[nodiscard]] bool SaveToPath(const std::string& path) const noexcept;
    [[nodiscard]] bool LoadFromPath(const std::string& path) noexcept;

    void SetData(VariableType* New);

public:
    VarStorageEntry(const VarStorageEntry& obj) = delete;
    VarStorageEntry(VarStorageEntry&& obj) = delete;
    ~VarStorageEntry();

    friend class VarStorage;

    VarStorageEntry& operator=(const VarStorageEntry& obj) = delete;
    VarStorageEntry& operator=(VarStorageEntry& Obj) = delete;

    [[nodiscard]] bool HasData() const noexcept;
    [[nodiscard]] const VariableType& Data() const;
    [[nodiscard]] VarStorageKey Key() const noexcept;
    [[nodiscard]] const std::string& Name() const noexcept;

    void Print(std::ostream& out) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const VarStorageEntry& obj) noexcept;

class VarStorage
{
private:
    std::vector<VarStorageEntry> entries;
    VarStorageKey curr_key = 0;
    VarStorageEntry ans;
    std::string root;
    bool save_to_files;

    [[nodiscard]] static std::string GetVariablePath(const std::string& name);

    [[nodiscard]] const VarStorageEntry* GetEntry(VarStorageKey key) const noexcept;
    [[nodiscard]] VarStorageEntry* GetEntry(VarStorageKey key) noexcept;

public:
    explicit VarStorage(std::string root);
    VarStorage(const VarStorage& obj) = delete;
    VarStorage(VarStorage&& obj) noexcept = delete;
    ~VarStorage();

    /*
     * To use this class, you mostly need to use the VarStorageKey provided by different operations.
     * To retrieve the value of ANS, you use (VarStorageKey)0.
     */

    VarStorage& operator=(const VarStorage& obj) = delete;
    VarStorage& operator=(VarStorage&& obj) noexcept = delete;

    [[nodiscard]] bool IsSavingToFiles() const noexcept;
    void SetSavingToFiles(bool New) noexcept;

    [[nodiscard]] bool IndexVariables() noexcept;
    void ReIndexKeys() noexcept; //Resets all keys to be linear. Only call if the number of keys is approaching the numeric limit.

    [[nodiscard]] std::optional<VarStorageKey> GetEntryKey(const std::string& name, VarEntryType type) noexcept;

    [[nodiscard]] bool DoesEntryExist(VarStorageKey key) const noexcept;
    [[nodiscard]] std::pair<const VariableType&, bool> GetEntryValue(VarStorageKey key) const noexcept;
    [[nodiscard]] bool SetEntryValue(VarStorageKey key) noexcept;

    [[nodiscard]] bool LoadEntry(VarStorageKey key) noexcept; //Loads a variable from a stored file, if it is stored there.
    [[nodiscard]] bool ResetEntry(VarStorageKey key) noexcept; //Clears the value from a specified entry.

    [[nodiscard]] bool PopEntry(VarStorageEntry key) noexcept; //Remove from current instance, but leaves it in file system.
    [[nodiscard]] bool DropEntry(VarStorageEntry key) noexcept; //Pop and remove from file system.

    [[nodiscard]] std::optional<VarStorageKey> AddEntry(const std::string& name, VarEntryType type, VariableType* data) noexcept;
};


#endif //JASON_VAR_STORAGE_H
