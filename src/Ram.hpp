#ifndef RAM_HPP
#define RAM_HPP

#include "Types.hpp"
#include <array>

constexpr std::size_t RamSize = 256 * 256;

class Ram
{
public:
    Ram();
    void Clear();

    Word &operator[](std::size_t index);

private:
    std::array<Word, RamSize> m_Data;
};

#endif
