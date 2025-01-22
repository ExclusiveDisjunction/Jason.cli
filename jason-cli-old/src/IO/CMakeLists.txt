add_library(IO SHARED
        Package.cpp
        Package.h
        Session.cpp
        Session.h
        PackageEntry.cpp
        PackageEntry.h
        PackageEntryKey.h
        FileHandle.cpp
        FileHandle.h
        UnloadedPackage.h
        UnloadedPackage.cpp
        PackageHeader.cpp
        PackageHeader.h
        PackageEntryIndex.h
        PackageEntryIndex.cpp
        PackageIndex.cpp
        PackageEntryKey.cpp
        PackagePager.cpp
        PackagePager.h)

target_link_libraries(IO Calc)
target_link_libraries(IO Core)