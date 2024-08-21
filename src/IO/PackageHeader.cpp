//
// Created by Hollan on 8/21/24.
//

#include "PackageHeader.h"

std::optional<PackageHeader> PackageHeader::FromFile(std::istream& in, std::optional<std::streamoff> endLoc)
{

}
PackageHeader::PackageHeader(std::optional<PackageHandle>&& handle, Version ver, std::optional<std::string> author, bool locked) : handle(std::move(handle)), ver(ver), author(std::move(author)), locked(locked)
{

}

void PackageHeader::Output(std::ostream& out) const noexcept
{

}
