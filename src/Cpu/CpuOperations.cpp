#include "Cpu.hpp"
#include "CpuBitwise.hpp"
#include "../Ram.hpp"

Word Cpu::Addition(DWord operand)
{
    DWord result = m_A + operand + m_Status.C;

    m_Status.C = result >= 0xFF;
    SET_NEGATIVE_FLAG(result);
    SET_ZERO_FLAG(result);
    m_Status.V = ~(m_A ^ operand) & (m_A ^ result) & NEGATIVE_BIT;

    return result & 0x00FF;
}

void Cpu::Branch(bool condition, DWord destination)
{
    if (condition)
    {
	m_RemainingCycles++;
	
        m_PC = destination;

	// Additional cycle if page crossed
	if((m_PC & 0xFF00) != (destination & 0xFF00))
	{
	    m_RemainingCycles++;
	}
    }
}

void Cpu::Compare(Word reg, Word operand)
{
    Word difference = reg - operand;

    SET_NEGATIVE_FLAG(difference);
    m_Status.Z = reg == operand;
    m_Status.C = reg >= operand;
}

Word Cpu::Increment(Word operand, bool positive)
{
    operand = (operand + (positive ? 1 : -1)) % 256;
    SET_ZERO_FLAG(operand);
    SET_NEGATIVE_FLAG(operand);
    return operand;
}

void Cpu::LoadRegister(Word &reg, Word value)
{
    reg = value;
    SET_ZERO_FLAG(value);
    SET_NEGATIVE_FLAG(value);
}

void Cpu::Return(Word offset)
{
    m_Status.value = PopWord();
    m_PC = PopDWord() + offset;
}

void Cpu::Transfer(Word &from, Word &to)
{
    to = from;
    SET_NEGATIVE_FLAG(from);
    SET_ZERO_FLAG(from);
}

void Cpu::ADC(DWord source)
{
    Word operand = FetchWord(source);
    m_A = Addition(operand);
}

void Cpu::AND(DWord source)
{
    Word operand = FetchWord(source);
    m_A &= operand;
    SET_ZERO_FLAG(m_A);
    SET_NEGATIVE_FLAG(m_A);
}

void Cpu::ASL(DWord source)
{
    Word operand = FetchWord(source);
    Word result = (operand << 1) & 0xFF;

    m_Status.C = operand & 0x80;
    SET_NEGATIVE_FLAG(result);
    SET_ZERO_FLAG(result);

    SetWord(source, result);
}

void Cpu::BCC(DWord destination)
{
    Branch(m_Status.C == 0, destination);
}

void Cpu::BCS(DWord destination)
{
    Branch(m_Status.C == 1, destination);
}

void Cpu::BEQ(DWord destination)
{
    Branch(m_Status.Z == 0, destination);
}

void Cpu::BIT(DWord source)
{
    Word operand = FetchWord(source);
    Word result = operand & m_A;

    SET_ZERO_FLAG(result);
    SET_NEGATIVE_FLAG(result);
    m_Status.V = operand & (1 << 6);
}

void Cpu::BMI(DWord destination)
{
    Branch(m_Status.N == 1, destination);
}

void Cpu::BNE(DWord destination)
{
    Branch(m_Status.Z == 0, destination);
}

void Cpu::BPL(DWord destination)
{
    Branch(m_Status.N == 0, destination);
}

void Cpu::BRK(DWord)
{
    m_PC++;

    PushDWord(m_PC);
    // Push the status register onto the stack with the break bit active
    PushWord(m_Status.value | (1 << 4));
    m_Status.I = 1;

    DWord pcLo = FetchWord(s_IrqVector);
    DWord pcHi = FetchWord(s_IrqVector + 1);

    m_PC = CONCATENATE_WORDS(pcLo, pcHi);
}

void Cpu::BVC(DWord destination)
{
    Branch(m_Status.V == 0, destination);
}

void Cpu::BVS(DWord destination)
{
    Branch(m_Status.V == 1, destination);
}

void Cpu::CLC(DWord)
{
    m_Status.C = 0;
}
void Cpu::CLD(DWord)
{
    m_Status.D = 0;
}
void Cpu::CLI(DWord)
{
    m_Status.I = 0;
}
void Cpu::CLV(DWord)
{
    m_Status.V = 0;
}

void Cpu::CMP(DWord source)
{
    Compare(m_A, FetchWord(source));
}
void Cpu::CPX(DWord source)
{
    Compare(m_X, FetchWord(source));
}
void Cpu::CPY(DWord source)
{
    Compare(m_X, FetchWord(source));
}

void Cpu::DEC(DWord source)
{
    Word result = Increment(FetchWord(source), false);
    SetWord(source, result);
}
void Cpu::DEX(DWord)
{
    m_X = Increment(m_X, false);
}
void Cpu::DEY(DWord)
{
    m_Y = Increment(m_Y, false);
}

void Cpu::EOR(DWord source)
{
    Word operand = FetchWord(source);
    m_A ^= operand;
    SET_NEGATIVE_FLAG(operand);
    SET_ZERO_FLAG(operand);
}

void Cpu::INC(DWord source)
{
    Word result = Increment(FetchWord(source), true);
    SetWord(source, result);
}
void Cpu::INX(DWord)
{
    m_X = Increment(m_X, true);
}
void Cpu::INY(DWord)
{
    m_Y = Increment(m_Y, true);
}

void Cpu::JMP(DWord destination)
{
    m_PC = destination;
}

void Cpu::JSR(DWord destination)
{
    PushDWord(--m_PC);
    m_PC = destination;
}

void Cpu::LDA(DWord source)
{
    LoadRegister(m_A, FetchWord(source));
}
void Cpu::LDX(DWord source)
{
    LoadRegister(m_X, FetchWord(source));
}
void Cpu::LDY(DWord source)
{
    LoadRegister(m_Y, FetchWord(source));
}

void Cpu::LSR(DWord source)
{
    Word operand = FetchWord(source);
    Word result = (operand >> 1) & 0xFF;

    m_Status.C = operand & 0x01;
    SET_NEGATIVE_FLAG(result);
    SET_ZERO_FLAG(result);

    SetWord(source, result);
}

void Cpu::NOP(DWord)
{
    return;
}

void Cpu::ORA(DWord source)
{
    Word operand = FetchWord(source);
    m_A |= operand;

    SET_ZERO_FLAG(m_A);
    SET_NEGATIVE_FLAG(m_A);

    SetWord(source, m_A);
}

void Cpu::PHA(DWord)
{
    PushWord(m_A);
}
void Cpu::PHP(DWord)
{
    PushWord(m_Status.value);
}

void Cpu::PLA(DWord)
{
    m_A = PopWord();
}
void Cpu::PLP(DWord)
{
    m_Status.value = PopWord();
}

void Cpu::ROL(DWord source)
{
    Word operand = FetchWord(source);
    DWord result = (operand << 1) | m_Status.C;

    m_Status.C = result > 0xFF;
    SET_NEGATIVE_FLAG(result & 0xFF);
    SET_ZERO_FLAG(result & 0xFF);

    SetWord(source, result);
}

void Cpu::ROR(DWord source)
{
    Word operand = FetchWord(source);
    Word result = (operand >> 1) | (m_Status.C << 7);

    m_Status.C = operand & 0x01;
    SET_NEGATIVE_FLAG(result);
    SET_ZERO_FLAG(result);

    SetWord(source, result);
}

void Cpu::RTI(DWord)
{
    Return(0);
}
void Cpu::RTS(DWord)
{
    Return(1);
}

void Cpu::SBC(DWord source)
{
    Addition(~FetchWord(source));
}

void Cpu::SEC(DWord)
{
    m_Status.C = 1;
}
void Cpu::SED(DWord)
{
    m_Status.D = 1;
}
void Cpu::SEI(DWord)
{
    m_Status.I = 1;
}

void Cpu::STA(DWord source)
{
    SetWord(source, m_A);
}
void Cpu::STX(DWord source)
{
    SetWord(source, m_X);
}
void Cpu::STY(DWord source)
{
    SetWord(source, m_Y);
}

void Cpu::TAX(DWord)
{
    Transfer(m_A, m_X);
}
void Cpu::TAY(DWord)
{
    Transfer(m_A, m_Y);
}
void Cpu::TSX(DWord)
{
    Transfer(m_SP, m_X);
}
void Cpu::TXA(DWord)
{
    Transfer(m_X, m_A);
}
void Cpu::TXS(DWord)
{
    Transfer(m_X, m_SP);
}
void Cpu::TYA(DWord)
{
    Transfer(m_Y, m_A);
}

void Cpu::ILL(DWord)
{
    return;
}
