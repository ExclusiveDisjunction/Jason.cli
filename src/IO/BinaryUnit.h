//
// Created by exdisj on 10/21/24.
//

#ifndef JASON_BINARYUNIT_H
#define JASON_BINARYUNIT_H

#include <vector>
#include <utility>
#include <memory.h>
#include <stdexcept>

class Unit
{
private:
    char* Data = nullptr;
    unsigned char blockSize = 0;

public:
    Unit();
    template<typename T>
    explicit Unit(const T& item);
    template<typename T>
    explicit Unit(const std::vector<T>& items);
    Unit(const Unit& obj) noexcept;
    Unit(Unit&& obj) noexcept;
    ~Unit();

    void Deallocate();
    template<typename T>
    void Allocate(const T& obj);
    template<typename T>
    void Allocate(const std::vector<T>& obj);

    Unit& operator=(const Unit& obj) noexcept;
    Unit& operator=(Unit&& obj) noexcept;

    template<typename T>
    [[nodiscard]] T Convert() const;
    template<typename T>
    void Convert(T& result) const;
    template<typename T>
    [[nodiscard]] std::vector<T> ConvertMany() const;
};

#include "BinaryUnit.tpp"

#endif //JASON_BINARYUNIT_H
