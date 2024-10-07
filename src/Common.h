//
// Created by exdisj on 8/10/24.
//

#ifndef JASON_COMMON_H
#define JASON_COMMON_H

#include <string>
#include <fstream>
#include <cstring>

inline void trim_left(std::string& item) noexcept
{
    auto iter = item.begin();
    for (; iter != item.end(); iter++)
    {
        if (!isspace(*iter))
            break;
    }

    item.erase(item.begin(), iter);
}
inline void trim_right(std::string& item) noexcept
{
    auto iter = item.rbegin();
    for (; iter != item.rend(); iter++)
    {
        if (!isspace(*iter))
            break;
    }

    item.erase(iter.base(), item.end());
}
inline void trim(std::string& item) noexcept
{
    trim_left(item);
    trim_right(item);
}

inline void readFileBlock(std::ostream& target, std::istream& input, std::streampos dest)
{
    if (!target || !input || dest <= input.tellg())
        return;

    std::streamoff blockSize = dest - input.tellg();
    char* buffer = new char[blockSize + 1];
    memset(buffer, 0, (blockSize + 1) * sizeof(char));
    input.read(buffer, blockSize);

    std::string temp(buffer);
    delete[] buffer;
    buffer = nullptr;

    trim(temp);
    target << temp;
}

#endif //JASON_COMMON_H
