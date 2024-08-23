//
// Created by exdisj on 8/8/24.
//

#ifndef JASON_PACKAGEUTILITY_H
#define JASON_PACKAGEUTILITY_H

#include <string>
#include <fstream>

class CompressedPackageIndex
{
public:
    CompressedPackageIndex() : CompressedPackageIndex(0, 0, 0, 0) {}
    CompressedPackageIndex(std::streampos header, std::streampos links, std::streampos entries, std::streampos functions) : header(header), links(links), entries(entries), functions(functions) {}

    std::streampos header, links, entries, functions;
};

#endif //JASON_PACKAGEUTILITY_H
