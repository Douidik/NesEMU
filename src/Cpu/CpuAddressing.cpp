#include "Cpu.hpp"
#include "CpuBitwise.hpp"
#include "../Ram.hpp"

DWord Cpu::IMP()
{
    return s_ImplicitSource;
}

DWord Cpu::IMM()
{
    return Read(m_PC++);
}

DWord Cpu::REL()
{
    DWord offset = (DWord)Read(m_PC++);

    if (offset & NEGATIVE_BIT)
    {
        offset |= 0xFF00;
    }

    return m_PC + offset;
}

DWord Cpu::ZeroPage(DWord offset)
{
    return (m_PC++ + offset) % 256;
}

DWord Cpu::ZER()
{
    return ZeroPage(0);
}
DWord Cpu::ZPX()
{
    return ZeroPage(m_X);
}
DWord Cpu::ZPY()
{
    return ZeroPage(m_Y);
}

DWord Cpu::Absolute(DWord offset)
{
    DWord lo = Read(m_PC++);
    DWord hi = Read(m_PC++);
    DWord address = CONCATENATE_WORDS(hi, lo) + offset;

    // Additional cycle if page crossed
    if((hi << 8) != (address & 0xFF00))
    {
	m_RemainingCycles++;
    }
    
    return address;
}

DWord Cpu::ABS()
{
    return Absolute(0);
}
DWord Cpu::ABX()
{
    return Absolute(m_X);
}
DWord Cpu::ABY()
{
    return Absolute(m_Y);
}

DWord Cpu::IND()
{
    DWord pointerLo = Read(m_PC++);
    DWord pointerHi = Read(m_PC++);

    DWord pointer = CONCATENATE_WORDS(pointerLo, pointerHi);
    
    if (pointerLo == 0x00FF)
    {
        return Read((pointer & 0xFF00) << 8) | Read(pointer);
    }
    else
    {
        return Read((pointer + 1) << 8) | Read(pointer);
    }
}

DWord Cpu::IDX()
{
    DWord zeroLo = (Read(m_PC++) + m_X) % 256;
    DWord zeroHi = (zeroLo + 1) % 256;
    return CONCATENATE_WORDS(Read(zeroLo), Read(zeroHi));
}

DWord Cpu::IDY()
{
    DWord zeroLo = Read(m_PC++);
    DWord zeroHi = (zeroLo + 1) % 256;
    return CONCATENATE_WORDS(Read(zeroLo), Read(zeroHi)) + m_Y;
}
