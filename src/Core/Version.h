#ifndef JASON_VERSION
#define JASON_VERSION

#include <iostream>

class Version
{
public:
    Version() : Version(0, 0, 0) {}
    Version(unsigned Major, unsigned Minor, unsigned Release) : Major(Major), Minor(Minor), Release(Release) {}

    unsigned Major;
    unsigned Minor;
    unsigned Release;

    bool operator==(const Version& obj) const noexcept;
    bool operator!=(const Version& obj) const noexcept;
};
std::ostream& operator<<(std::ostream& out, const Version& obj) noexcept;
std::istream& operator>>(std::istream& in, Version& obj);

#define JASON_VERSION_1_0_0 (Version(1, 0, 0))
#define JASON_CURRENT_VERSION JASON_VERSION_1_0_0

#endif