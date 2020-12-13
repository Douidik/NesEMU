#include "Cpu.hpp"
#include "CpuBitwise.hpp"
#include "../Ram.hpp"

void Cpu::Interrupt(DWord interruptVector)
{
    m_Status.B = 0;
    PushWord(m_Status.value);
    PushDWord(m_PC);
    m_Status.I = 1;

    DWord programLo = Read(interruptVector);
    DWord programHi = Read(interruptVector + 1);
    
    m_PC = CONCATENATE_WORDS(programHi, programLo);
}

void Cpu::IRQ()
{
    if (!m_Status.I)
    {
        Interrupt(s_IrqVector);
    }

    m_RemainingCycles = 7;
}

void Cpu::NMI()
{
    Interrupt(s_NmiVector);
    m_RemainingCycles = 8;
}

void Cpu::Reset()
{
    DWord programLo = Read(s_ResetVector);
    DWord programHi = Read(s_ResetVector + 1);
    
    m_A = m_X = m_Y = 0x00;
    m_PC = 0x0000;
    m_SP = 0xFD;
    
    m_Status.value = 0x00;
    m_Status.U = 1;

    m_RemainingCycles = 8;
}
