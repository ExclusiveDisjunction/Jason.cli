#include <iostream>
#include <filesystem>
#include <fstream>
#include <utility>

#include "Filehandle.h"

//Serves to store the package's index, by listing off its iterators. 
class PackageIndex 
{
private:
    FileHandle handle;

public:
    PackageIndex(FileHandle&& handle);

    void Close();
};