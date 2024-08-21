//
// Created by exdisj on 8/12/24.
//

#include "PackageLink.h"
#include "PackageHandle.h"

#include <utility>

PackageLink::PackageLink(PackageHandle&& host, bool enabled) : host(std::move(host)), enabled(enabled)
{

}

PackageLink::PackageLink(PackageLink&& obj) noexcept : host(std::move(obj.host)), enabled(std::exchange(obj.enabled, false))
{

}

PackageHandle& PackageLink::GetHost() noexcept
{
    return this->host;
}
const std::filesystem::path& PackageLink::GetPath() const noexcept
{
    return this->host.path;
}
bool PackageLink::IsEnabled() const noexcept
{
    return this->enabled;
}
