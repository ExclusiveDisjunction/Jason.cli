//
// Created by exdisj on 8/12/24.
//

#ifndef JASON_PACKAGELINK_H
#define JASON_PACKAGELINK_H

#include "PackageHandle.h"

class PackageLink
{
private:
    PackageHandle host;
    bool enabled;

public:
    PackageLink(PackageHandle&& host, bool enabled);
    PackageLink(const PackageLink& obj) = delete;
    PackageLink(PackageLink&& obj) noexcept;

    PackageLink& operator=(const PackageLink& obj) = delete;
    PackageLink& operator=(PackageLink&& obj) noexcept = delete;

    [[nodiscard]] PackageHandle& GetHost() noexcept;
    [[nodiscard]] const std::filesystem::path& GetPath() const noexcept;
    [[nodiscard]] bool IsEnabled() const noexcept;
};


#endif //JASON_PACKAGELINK_H
