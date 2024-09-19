#include <iostream>
#include <filesystem>
#include <fstream>
#include <utility>

#include "FileHandle.h"

//Serves to store the package's index, by listing off its iterators. 
class PackageIndex 
{
private:
    FileHandle handle;

public:
    PackageIndex(FileHandle&& handle);

    void Close();
};