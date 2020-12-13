#include "Cpu.hpp"
#include "CpuBitwise.hpp"
#include "../Ram.hpp"

Cpu::Cpu(Ram &ram) : m_Ram(ram)
{
    GenerateInstructionSet();
}

void Cpu::Clock()
{
    while (m_RemainingCycles <= 0)
    {
        Word opcode = Read(m_PC++);
        Instruction &instruction = m_InstructionSet[opcode];
	
        DWord source = instruction.addressing();
        instruction.operation(source);

        m_RemainingCycles += instruction.cycles;
    }

    m_RemainingCycles--;
}


Word Cpu::Read(DWord address)
{
    return m_Ram[address];
}

Word Cpu::Write(DWord address, Word value)
{
    return m_Ram[address] = value;
}

Word Cpu::PushWord(Word value)
{
    return Write(s_StackBase | m_SP--, value);
}
Word Cpu::PopWord()
{
    return Read(s_StackBase | ++m_SP);
}

DWord Cpu::PushDWord(DWord value)
{
    PushWord((Word)(value >> 8));
    PushWord((Word)(value & 0x00FF));
    return value;
}

DWord Cpu::PopDWord()
{
    Word lo = PopWord();
    Word hi = PopWord();
    return CONCATENATE_WORDS(hi, lo);
}

Word Cpu::FetchWord(DWord source)
{
    return source == s_ImplicitSource ? m_A : m_Ram[source];
}

Word Cpu::SetWord(DWord address, Word value)
{
    if (address == s_ImplicitSource)
    {
        return (m_A = value);
    }
    else
    {
        return Write(address, value);
    }
}
