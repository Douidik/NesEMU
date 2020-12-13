#include "Ram.hpp"
#include <iostream>
#include <stdexcept>

Word &Ram::operator[](std::size_t index)
{
    try
    {
        return m_Data[index];
    }
    catch (const std::out_of_range &error)
    {
        std::cerr << error.what() << std::endl;
        // Returning dummy value if an error occured
        return m_Data[RamSize - 1];
    }
}

