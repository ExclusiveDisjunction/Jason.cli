//
// Created by exdisj on 10/21/24.
//

#include "PackagePager.h"
#include "PackageEntry.h"

PackagePager::PackagePager(FileHandle&& location, unsigned char UnitSize, unsigned PageSize) : handle(std::move(location)), binding(), location(std::make_pair(0u, 0u)), unitSize(UnitSize), pageSize(PageSize)
{
    handle.file.seekg(0, std::ios::beg);
}
PackagePager::PackagePager(PackagePager&& obj) noexcept : handle(std::move(obj.handle)), binding(std::move(obj.binding)), location(std::move(obj.location)), unitSize(std::exchange(obj.unitSize, 0)), pageSize(std::exchange(obj.pageSize, 0))
{

}
PackagePager::~PackagePager()
{
    handle.Close();
    binding = {};
    location = std::make_pair(0u, 0u);
    unitSize = 0;
    pageSize = 0;
}

void PackagePager::ReviewKnownElements(const std::vector<PackageEntry>& obj)
{
    if (this->IsBound())
        this->Reset();

    this->knownPages.clear();
    
    for (const auto& element : obj)
    {
        for (const auto& index : element.GetIndex().pages)
            knownPages[index] = true;
    }
}

PackagePager& PackagePager::operator=(PackagePager&& obj) noexcept
{
    Close();

    this->handle = std::move(obj.handle);
    this->binding = std::move(obj.binding);
    this->location = std::move(obj.location);
    this->unitSize = std::exchange(obj.unitSize, 0);
    this->pageSize = std::exchange(obj.pageSize, 0);
    return *this;
}

unsigned char PackagePager::UnitSize() const noexcept
{
    return this->unitSize;
}
unsigned PackagePager::PageSize() const noexcept
{
    return this->pageSize;   
}

bool PackagePager::EndOfFile() const noexcept
{
    return handle.file.eof() || boundEof;
}
bool PackagePager::IsBound() const noexcept
{
    return binding.has_value() && boundPageIndex.has_value();
}

bool PackagePager::Allocate(unsigned int pages, PackageEntryIndex& tPages)
{
    if (IsBound())
        return false;

    std::vector<unsigned int>& Pages = tPages.pages;
    
    if (Pages.size() == pages)
        return true;

    if (pages == 0)
    {
        for (const auto& index: Pages)
            knownPages[index] = false;
        Pages.clear();
    }
    else if (pages < Pages.size()) //We need to shrink
    {
        unsigned amount = Pages.size() - pages;
        std::vector<unsigned int> NewPages = std::vector<unsigned int>(Pages.begin(), Pages.begin() + amount),
                ToRemove = std::vector<unsigned int>(Pages.begin() + amount, Pages.end());
        
        Pages = NewPages;
        
        for (const auto& index : NewPages)
            knownPages[index] = false;
    }
    else //Greater
    {
        //Move to end
        this->handle.file.seekg(0, std::ios::end);
        unsigned page = this->handle.file.tellg() / (unitSize * pageSize);
        page++;
        
        unsigned amount = pages - Pages.size();
        unsigned blockSize = unitSize * pageSize;
        char * chars = new char[blockSize];
        memset(chars, 0, blockSize);
        for (unsigned i = 0; i < amount; i++, page++)
        {
            knownPages[page] = true;
            Pages.emplace_back(page);
            handle.file.write(chars, blockSize);
        }

        delete[] chars;
    }

    return true;
}
std::vector<unsigned int> PackagePager::Allocate(unsigned int pages)
{
    if (this->IsBound() || !this->handle.file)
        return {};

    this->handle.file.seekg(0, std::ios::end);
    //Get the last page
    auto lastPage = std::max_element(this->knownPages.begin(), this->knownPages.end(), [](const auto& a, const auto& b) -> bool {
        return a < b;
    });

    unsigned int curr = 0;
    if (lastPage == this->knownPages.end())
        curr = 0;
    else
        curr = lastPage->first + 1;
        
    std::vector<unsigned int> result;
    for (unsigned i = 0; i < pages; i++, curr++)
    {
        result.push_back(curr);
        this->knownPages[curr] = true;
    }

    this->binding = &result;
    boundPageIndex = 0;
    bool couldWipe = this->WipeAll();

    Reset();
    if (!couldWipe)
        return {};
    else
        return result;
}

Unit PackagePager::ReadUnit()
{
    if (!IsBound() || EndOfFile())
        throw std::logic_error("Invalid operation: The reader is not bound, or is EOF.");

    char* data = new char[unitSize];
    this->handle.file.read(data, unitSize);

    //Now we need to check our location
    if (!Advance())
        throw std::logic_error("Could not advance");

    return { data, unitSize, false };
}
bool PackagePager::Advance()
{
    if (!IsBound() || EndOfFile())
        return false;

    location.second++;
    if (location.second / pageSize > 0) // We have moved to the next page
        return AdvancePage();
    else
        return true;
}
bool PackagePager::AdvancePage()
{
    if (!IsBound() || EndOfFile())
        return false;

    auto& index = boundPageIndex.value();
    auto& list = *binding.value();
    index++;

    if (index >= list.size()) //Out of range
    {
        boundEof = true;
        MoveAbsolute(0, 0);
    }
    else
    {
        unsigned nextPage = list[index];
        MoveAbsolute(nextPage, 0);
    }

    return !boundEof;
}
std::vector<Unit> PackagePager::ReadUnits(unsigned int Units)
{
    std::vector<Unit> result;
    result.resize(Units);

    try
    {
        for (unsigned i = 0; i < Units; i++)
            result[i] = std::move(ReadUnit());

        return result;
    }
    catch (...)
    {
        return {};
    }
}
std::vector<Unit> PackagePager::ReadAllUnits()
{
    if (!IsBound() || EndOfFile())
        throw std::logic_error("No information can be found.");

    MoveRelative(0u);
    std::vector<Unit> result;
    unsigned size = (*binding.value()).size() * pageSize;
    result.resize(size);

    unsigned i = 0;
    //The approach is to read page by page, selecting an array out of the stream one at a time.
    do
    {
        auto thisSize = unitSize * pageSize;
        char* thisPage = new char[thisSize];
        memset(thisPage, 0, thisSize);
        handle.file.read(thisPage, thisSize);

        for (unsigned j = 0; j < pageSize && i < size; i++, j++)
            result[i].Allocate(thisPage + (j * unitSize), unitSize, true);

        delete[] thisPage;
    } while (AdvancePage());

    return result;
}
bool PackagePager::WriteUnits(const std::vector<Unit>& units)
{
    if (!IsBound() || EndOfFile())
        return false;

    auto curr = units.begin(), end = units.end();
    do
    {
        if (curr->GetSize() != unitSize)
            return false;

        handle.file.write(curr->Expose(), unitSize);
        curr++;
    } while (Advance() && curr != end);

    return curr == end; //We know all elements were written.
}
bool PackagePager::WipeAll()
{
    if (!MoveRelative(0))
        return false;

    if (!IsBound() || EndOfFile())
        return false;

    char* sequence = new char[unitSize];
    memset(sequence, 0, unitSize);

    do
    {
        handle.file.write(sequence, unitSize);
    } while (Advance());

    delete[] sequence;
    return MoveRelative(0);
}

void PackagePager::Bind(PackageEntryIndex& pages)
{
    Reset();
    binding = &pages.pages;
    boundPageIndex = 0;
    MoveRelative(0);
}
void PackagePager::Reset()
{
    binding = {};
    boundPageIndex = {};
    boundEof = false;
    (void)MoveAbsolute(0, 0);
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
        MoveAbsolute(0, 0);
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

    unsigned pageLoc = unitPosition / pageSize;
    unitPosition -= pageLoc * pageSize;

    //We are moving from the first unit in the bound element.
    auto& pages = *(*binding);
    if (pageLoc > pages.size())
        return false;
    
    boundPageIndex = pageLoc;
    return MoveAbsolute(pages[pageLoc], unitPosition);
}
bool PackagePager::MoveAbsolute(unsigned pagePosition, unsigned unitPosition)
{
    return MoveAbsolute(std::make_pair(pagePosition, unitPosition));
}
bool PackagePager::MoveAbsolute(std::pair<unsigned, unsigned> loc)
{
    if (location == loc)
        return true; //Already there

    std::streamoff truePos = (loc.first * pageSize + loc.second) * unitSize;
    handle.file.seekg(truePos);

    if (handle.file.bad() || handle.file.eof()) //EOF
    {
        location = std::make_pair<unsigned, unsigned>(0, 0);
        if (boundPageIndex)
            boundPageIndex = 0; //Reset this if we fail
        return false;
    }
    else
    {
        location = loc;
        return true;
    }
}
unsigned PackagePager::GetRelativePosition()
{
    if (!IsBound() || EndOfFile())
        return UINT_MAX;
    
    return boundPageIndex.value() * pageSize + location.second;
}
std::pair<unsigned, unsigned> PackagePager::GetAbsolutePosition()
{
    return location;
}
