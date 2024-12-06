//
// Created by exdisj on 10/21/24.
//

#include "PackagePager.h"
#include "PackageEntry.h"

#include <sstream>

PackagePager::PackagePager(FileHandle&& location, FileHandle&& index_loc, unsigned char unit_size) : handle(std::move(location)), index(std::move(index_loc)), binding(), abs_location(0), unit_size(unit_size)
{
    handle.file.seekg(0, std::ios::beg);
}
PackagePager::PackagePager(PackagePager&& obj) noexcept : handle(std::move(obj.handle)), index(std::move(obj.index)), binding(std::move(obj.binding)), abs_location(std::exchange(obj.abs_location, 0)), unit_size(std::exchange(obj.unit_size, 0)), locations(std::move(obj.locations)), open_pages(std::move(obj.open_pages))
{

}
PackagePager::~PackagePager()
{
    this->Close();
}

PackagePager& PackagePager::operator=(PackagePager&& obj) noexcept
{
    Close();

    this->handle = std::move(obj.handle);
    this->binding = std::move(obj.binding);
    this->unit_size = std::exchange(obj.unit_size, 0);
    this->abs_location = std::exchange(obj.abs_location, 0);
    this->locations = std::move(obj.locations);
    this->open_pages = std::move(obj.open_pages);
    return *this;
}

bool PackagePager::ReadIndex() 
{
    if (this->IsBound() || index.file.bad())
        return false;

    std::string header;
    std::fstream& in = index.file;

    in >> header;
    if (header != "OPEN")
        return false;

    unsigned len;
    in >> len;
    std::vector<unsigned> open_indexes_tmp;
    open_indexes_tmp.resize(len, 0);

    for (unsigned i = 0; i < len && in.good(); i++)
        in >> open_indexes_tmp[i];
    
    if (in.bad())
        return false;

    in >> header;
    if (header != "INDX")
        return false;

    std::unordered_map<PackageEntryKey, std::vector<unsigned>> locations_tmp;
    std::string line;
    while (!in.eof())
    {
        std::getline(in, line);
        std::stringstream line_in(line);

        unsigned packID, entryID, line_size;
        line_in >> packID >> entryID >> line_size;
        std::vector<unsigned> indexes;
        indexes.resize(line_size, 0);
        for (unsigned i = 0; i < line_size && line_in.good(); i++) 
            line_in >> indexes[i];

        PackageEntryKey key(packID, entryID);
        locations_tmp[key] = std::move(indexes);
    }

    this->locations = std::move(locations_tmp);
    this->open_pages = std::priority_queue<unsigned, std::vector<unsigned>, std::greater<unsigned>>(open_indexes_tmp.begin(), open_indexes_tmp.end());

    return true;
}
bool PackagePager::SaveIndex()
{
    if (index.file.bad())
        return false;

    try
    {
        index.ReOpen(std::ios::in | std::ios::out | std::ios::trunc);
    }
    catch(...)
    {
        return false;
    }
    
    std::fstream& out = index.file;
    out << "OPEN" << ' ' << open_pages.size();
    for (const auto& index : open_pages.__get_container())
        out << ' ' << index;

    out << '\n';

    out << "INDX" << '\n';
    for (const auto& [key, indexes] : locations)
    {
        out << key.PackageID << ' ' << key.EntryID << ' ' << indexes.size();
        for (const auto& index : indexes)
            out << ' ' << index;
        out << '\n';
    }

    return true;
}

unsigned char PackagePager::UnitSize() const noexcept
{
    return this->unit_size;
}

bool PackagePager::IsEOF() const noexcept
{
    return handle.file.eof() || (!this->binding.has_value() || this->binding->eof);
}
bool PackagePager::IsBound() const noexcept
{
    return binding.has_value();
}

bool PackagePager::Allocate(unsigned int units, PackageEntryKey key)
{
    std::vector<unsigned>& curr_alloc = this->locations[key];

    std::vector<unsigned> removed_units; //The units that were removed, to add to open_pages

    if (curr_alloc.size() == units) 
        return true;
    else if (units == 0)
        removed_units = std::move(curr_alloc); //The curr_alloc will hold no more data.
    else if (curr_alloc.size() < units) //Adding units
    {
        unsigned to_add = units - curr_alloc.size();
        std::vector<unsigned> new_units = this->RequestUnits(to_add);
        if (new_units.size() != to_add)
            return false; //Request failed

        for (const auto& unit : new_units)
            curr_alloc.push_back(unit);
    }
    else if (curr_alloc.size() > units) //Remove units 
    {
        unsigned to_remove = curr_alloc.size() - units;
        for (auto curr = curr_alloc.rbegin(); to_remove > 0 && curr != curr_alloc.rend(); to_remove--)
            removed_units.push_back(*curr);
        
        curr_alloc.resize(units);
    }

    for (const auto& unit : removed_units)
        this->open_pages.push(unit);

    return true;
}
bool PackagePager::AllocateOnBound(unsigned units)
{
    if (!IsBound())
        return false;

    PackagePagerBinding bind(std::move(*binding));
    bool result = this->Allocate(units, bind.key);
    
    this->binding = std::move(bind);
    return this->MoveRelative(this->binding->offset) && result;
}
bool PackagePager::Register(PackageEntryKey key)
{
    if (!this->locations.contains(key))
    {
        std::vector<unsigned> requested = RequestUnits(1);
        if (requested.empty()) //Failed to request a page
            return false;

        this->locations[key] = requested;
    }

    return true;
}
std::vector<unsigned> PackagePager::RequestUnits(unsigned units)
{
    std::vector<unsigned> result;
    while (!open_pages.empty() && units > 0) 
    {
        result.push_back(open_pages.top());
        open_pages.pop();
        units--;
    }

    if (units != 0) 
    {
        //We still have more to satisfy
        
        //We need to get the last unit position, and then add one to it.
        unsigned last_pos = 0;
        for (const auto& [key, locs] : locations)
        {
            if (locs.empty())
                continue;

            last_pos = std::max(
                last_pos,
                *std::max_element(locs.begin(), locs.end())
            );
        }

        if (!locations.empty()) //If we have nothing, we dont increment to the end, because we are already at the end (last_pos = 0)
            last_pos++;
        

        this->handle.file.seekp(0, std::ios::end);
        unsigned block_size = units * unit_size;
        char* chunk = new char[block_size]; 
        memset(chunk, 0, block_size * sizeof(char));

        this->handle.file.write(chunk, block_size);

        for (unsigned i = 0; i < units; i++)
            result.push_back(last_pos++);

        delete[] chunk;

        if (this->handle.file.bad())
            return {}; //Error
    }

    return result;
}
void PackagePager::RemoveAllocation(PackageEntryKey key)
{
    auto iter = std::find_if(locations.begin(), locations.end(), [&key](const auto& item) -> bool {
        return item.first == key;
    });

    if (iter == locations.end())
        return;

    auto& [l_k, list] = *iter;
    for (const auto& index : list)
        open_pages.push(index);
    list.clear();

    locations.erase(iter);
}
bool PackagePager::EnsureAllocation(unsigned needed)
{
    if (!IsBound())
        return false;

    const auto& allocation = locations[binding->key];
    if (allocation.size() != needed)
        return AllocateOnBound(needed);
    else
        return true;
}

bool PackagePager::Advance()
{
    if (!IsBound() || IsEOF())
        return false;

    binding->offset++;
    if (binding->offset >= locations[binding->key].size()) //We have reached EOF
        binding->eof = true; //Note that it is ok that we advance, but we set the EOF flag. This function should only fail if the move relative failed, or the reader is not bound or in EOF.
    else 
        return MoveRelative(binding->offset);

    return true;
}
Unit PackagePager::ReadUnit()
{
    if (!IsBound())
        throw std::logic_error("reader is not bound");
    if (IsEOF())
        throw std::logic_error("end of file");

    char* data = new char[unit_size];
    memset(data, 0, sizeof(char) * unit_size);

    this->handle.file.read(data, unit_size);
    if (!Advance())
        throw std::logic_error("could not advance to the next unit");

    return { data, unit_size, false };
}
std::vector<Unit> PackagePager::ReadUnits(unsigned int Units)
{
    if (Units == 0)
        return {};

    std::vector<Unit> result;
    result.resize(Units);

    for (unsigned i = 0; i < Units; i++)
        result[i] = std::move(ReadUnit());

    return result;
}
std::vector<Unit> PackagePager::ReadAllUnits()
{
    if (IsBound() && locations[binding->key].size() == 0) //empty list
        return {};

    return ReadUnits(locations[binding->key].size());
}
bool PackagePager::WriteUnits(const std::vector<Unit>& units)
{
    if (!IsBound() || IsEOF())
        return false;

    auto curr = units.begin(), end = units.end();
    do
    {
        if (curr->GetSize() != unit_size)
            return false;

        handle.file.write(curr->Expose(), unit_size);
        curr++;
    } while (Advance() && curr != end);

    return curr == end; //We know all elements were written.
}
bool PackagePager::WipeAll()
{
    if (!MoveRelative(0))
        throw std::logic_error("not currently bound or element has no pages allocated to it");

    Unit zero_unit(unit_size);

    do
    {
        handle.file.write(zero_unit.Expose(), unit_size);
    } while (Advance());

    return MoveRelative(0);
}

bool PackagePager::Bind(PackageEntryKey key)
{
    if (!this->locations.contains(key))
    {
        if (!Register(key))
            return false;
    }


    binding = PackagePagerBinding(key, 0);
    return MoveRelative(0);
}
void PackagePager::Reset()
{
    binding = {};
    (void)MoveAbsolute(0);
}
void PackagePager::Close()
{
    Reset();
    handle.Close();
}
void PackagePager::Flush()
{
    handle.file.flush();
}
void PackagePager::TruncateFile()
{
    std::filesystem::path this_path = this->handle.path;
    this->Close();

    try
    {
        this->handle.Open(this_path, std::ios::in | std::ios::out | std::ios::trunc);
        MoveAbsolute(0);
    }
    catch (...)
    {
        //Do nothing?
    }
}

bool PackagePager::MoveRelative(unsigned unitPosition)
{
    if (!IsBound())
        return false;

    const std::vector<unsigned>& curr_alloc = this->locations[binding->key];
    if (unitPosition > curr_alloc.size())
        return false;

    unsigned unit_loc = curr_alloc[unitPosition];
    return MoveAbsolute(unit_loc);
}
bool PackagePager::MoveAbsolute(unsigned loc)
{
    std::streamoff truePos = loc * unit_size;
    if (truePos == handle.file.tellg())
    {
        abs_location = loc; //Ensure that we update our internal counts
        return true; //Already there in the file
    }

    handle.file.seekg(truePos);

    abs_location = loc;

    if (handle.file.bad() || handle.file.eof()) //EOF
    {
        if (binding.has_value())
            binding->eof = true;
        
        return false;
    }

    return true;
}
std::optional<unsigned> PackagePager::GetRelativePosition() const noexcept
{
    if (!IsBound() || IsEOF())
        return {};
    
    return binding->offset;
}
unsigned PackagePager::GetAbsolutePosition() const noexcept
{
    return abs_location;
}