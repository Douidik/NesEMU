#ifndef BUS_HPP
#define BUS_HPP

#include "Ram.hpp"
#include "Cpu/Cpu.hpp"
#include "Ppu.hpp"

class Bus
{
public:
    Bus();
    
private:
    Cpu m_Cpu;
    Ppu m_Ppu;
    Ram m_Ram;
}; 

#endif
