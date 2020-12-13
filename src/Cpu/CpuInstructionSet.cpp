#include "Cpu.hpp"

void Cpu::GenerateInstructionSet()
{

#define INS(cycles, addressing, operation) \
    { cycles, std::bind(&Cpu::addressing, this), std::bind(&Cpu::operation, this, std::placeholders::_1) }

    static Instruction s_DefaultInstruction = INS(0, IMP, ILL);
    std::fill(m_InstructionSet.begin(), m_InstructionSet.end(), s_DefaultInstruction);

#define DEF_INS(opcode, cycles, addressing, operation)	    \
    static_assert(opcode < 256, "Opcode exceed 256 limit"); \
    m_InstructionSet[opcode] = INS(cycles, addressing, operation)

    DEF_INS(0x69, 2, IMM, ADC);
    DEF_INS(0x65, 3, ZER, ADC);
    DEF_INS(0x75, 4, ZPX, ADC);
    DEF_INS(0x6D, 4, ABS, ADC);
    DEF_INS(0x7D, 4, ABX, ADC);
    DEF_INS(0x79, 4, ABY, ADC);
    DEF_INS(0x61, 6, IDX, ADC);
    DEF_INS(0x71, 5, IDY, ADC);

    DEF_INS(0x29, 2, IMM, AND);
    DEF_INS(0x25, 3, ZER, AND);
    DEF_INS(0x35, 4, ZPX, AND);
    DEF_INS(0x2D, 4, ABS, AND);
    DEF_INS(0x3D, 4, ABX, AND);
    DEF_INS(0x39, 4, ABY, AND);
    DEF_INS(0x21, 6, IDX, AND);
    DEF_INS(0x31, 5, IDY, AND);

    DEF_INS(0x0A, 2, IMP, ASL);
    DEF_INS(0x06, 5, ZER, ASL);
    DEF_INS(0x16, 6, ZPX, ASL);
    DEF_INS(0x0E, 6, ABS, ASL);
    DEF_INS(0x1E, 7, ABX, ASL);

    DEF_INS(0x90, 2, REL, BCC);
    DEF_INS(0xB0, 2, REL, BCS);
    DEF_INS(0xF0, 2, REL, BEQ);

    DEF_INS(0x24, 3, ZER, BIT);
    DEF_INS(0x2C, 4, ABS, BIT);

    DEF_INS(0xD0, 2, REL, BNE);
    DEF_INS(0x10, 2, REL, BPL);

    DEF_INS(0x00, 7, IMP, BNE);

    DEF_INS(0x50, 2, REL, BVC);
    DEF_INS(0x70, 2, REL, BVS);

    DEF_INS(0x18, 2, IMP, CLC);
    DEF_INS(0xD8, 2, IMP, CLD);
    DEF_INS(0x58, 2, IMP, CLI);
    DEF_INS(0xB8, 2, IMP, CLV);

    DEF_INS(0xC9, 2, IMM, CMP);
    DEF_INS(0xC5, 3, ZER, CMP);
    DEF_INS(0xD5, 4, ZPX, CMP);
    DEF_INS(0xCD, 4, ABS, CMP);
    DEF_INS(0xDD, 4, ABX, CMP);
    DEF_INS(0xD9, 4, ABY, CMP);
    DEF_INS(0xC1, 6, IDX, CMP);
    DEF_INS(0xD1, 5, IDY, CMP);

    DEF_INS(0xE0, 2, IMM, CPX);
    DEF_INS(0xE4, 3, ZER, CPX);
    DEF_INS(0xEC, 4, ABS, CPX);

    DEF_INS(0xC0, 2, IMM, CPY);
    DEF_INS(0xC4, 3, ZER, CPY);
    DEF_INS(0xCC, 4, ABS, CPY);

    DEF_INS(0xC6, 5, ZER, DEC);
    DEF_INS(0xD6, 6, ZPX, DEC);
    DEF_INS(0xCE, 6, ABS, DEC);
    DEF_INS(0xDE, 7, ABX, DEC);

    DEF_INS(0xCA, 2, IMP, DEX);
    DEF_INS(0x88, 2, IMP, DEY);

    DEF_INS(0x49, 2, IMM, EOR);
    DEF_INS(0x45, 3, ZER, EOR);
    DEF_INS(0x55, 4, ZPX, EOR);
    DEF_INS(0x4D, 4, ABS, EOR);
    DEF_INS(0x5D, 4, ABX, EOR);
    DEF_INS(0x59, 4, ABY, EOR);
    DEF_INS(0x41, 6, IDX, EOR);
    DEF_INS(0x51, 5, IDY, EOR);

    DEF_INS(0xE6, 5, ZER, DEC);
    DEF_INS(0xF6, 6, ZPX, DEC);
    DEF_INS(0xEE, 6, ABS, DEC);
    DEF_INS(0xFE, 7, ABX, DEC);

    DEF_INS(0xE8, 2, IMP, INX);
    DEF_INS(0xC8, 2, IMP, INY);

    DEF_INS(0x4C, 3, ABS, JMP);
    DEF_INS(0x6C, 5, IND, JMP);

    DEF_INS(0x20, 6, ABS, JSR);

    DEF_INS(0xA9, 2, IMM, LDA);
    DEF_INS(0xA5, 3, ZER, LDA);
    DEF_INS(0xB5, 4, ZPX, LDA);
    DEF_INS(0xAD, 4, ABS, LDA);
    DEF_INS(0xBD, 4, ABX, LDA);
    DEF_INS(0xB9, 4, ABY, LDA);
    DEF_INS(0xA1, 6, IDX, LDA);
    DEF_INS(0xB1, 5, IDY, LDA);

    DEF_INS(0xA2, 2, IMM, LDX);
    DEF_INS(0xA6, 3, ZER, LDX);
    DEF_INS(0xB6, 4, ZPY, LDX);
    DEF_INS(0xAE, 4, ABS, LDX);
    DEF_INS(0xBE, 4, ABY, LDX);

    DEF_INS(0xA0, 2, IMM, LDY);
    DEF_INS(0xA4, 3, ZER, LDY);
    DEF_INS(0xB4, 4, ZPX, LDY);
    DEF_INS(0xAC, 4, ABS, LDY);
    DEF_INS(0xBC, 4, ABX, LDY);

    DEF_INS(0x4A, 2, IMM, LSR);
    DEF_INS(0x46, 5, ZER, LSR);
    DEF_INS(0x56, 6, ZPX, LSR);
    DEF_INS(0x4E, 6, ABS, LSR);
    DEF_INS(0x5E, 7, ABX, LSR);

    DEF_INS(0xEA, 2, IMP, NOP);

    DEF_INS(0x09, 2, IMM, ORA);
    DEF_INS(0x05, 3, ZER, ORA);
    DEF_INS(0x15, 4, ZPX, ORA);
    DEF_INS(0x0D, 4, ABS, ORA);
    DEF_INS(0x1D, 4, ABX, ORA);
    DEF_INS(0x19, 4, ABY, ORA);
    DEF_INS(0x01, 6, IDX, ORA);
    DEF_INS(0x11, 5, IDY, ORA);

    DEF_INS(0x48, 3, IMP, PHA);
    DEF_INS(0x08, 3, IMP, PHP);

    DEF_INS(0x68, 4, IMP, PLA);
    DEF_INS(0x28, 4, IMP, PLP);

    DEF_INS(0X2A, 2, IMM, ROL);
    DEF_INS(0x26, 5, ZER, ROL);
    DEF_INS(0x36, 6, ZPX, ROL);
    DEF_INS(0x2E, 6, ABS, ROL);
    DEF_INS(0x3E, 7, ABX, ROL);

    DEF_INS(0X6A, 2, IMM, ROR);
    DEF_INS(0x66, 5, ZER, ROR);
    DEF_INS(0x76, 6, ZPX, ROR);
    DEF_INS(0x6E, 6, ABS, ROR);
    DEF_INS(0x7E, 7, ABX, ROR);

    DEF_INS(0x40, 6, IMP, RTI);
    DEF_INS(0x60, 6, IMP, RTS);

    DEF_INS(0xE9, 2, IMM, SBC);
    DEF_INS(0xE5, 3, ZER, SBC);
    DEF_INS(0xF5, 4, ZPX, SBC);
    DEF_INS(0xED, 4, ABS, SBC);
    DEF_INS(0xFD, 4, ABX, SBC);
    DEF_INS(0xF9, 4, ABY, SBC);
    DEF_INS(0xE1, 6, IDX, SBC);
    DEF_INS(0xF1, 5, IDY, SBC);

    DEF_INS(0x38, 2, IMP, SEC);
    DEF_INS(0xF8, 2, IMP, SED);
    DEF_INS(0x78, 2, IMP, SEI);

    DEF_INS(0x85, 3, ZER, STA);
    DEF_INS(0x95, 4, ZPX, STA);
    DEF_INS(0x8D, 4, ABS, STA);
    DEF_INS(0x9D, 5, ABX, STA);
    DEF_INS(0x99, 5, ABY, STA);
    DEF_INS(0x81, 6, IDX, STA);
    DEF_INS(0x91, 6, IDY, STA);

    DEF_INS(0x86, 3, ZER, STX);
    DEF_INS(0x96, 4, ZPY, STX);
    DEF_INS(0x8E, 4, ABS, STX);

    DEF_INS(0x84, 3, ZER, STY);
    DEF_INS(0x94, 4, ZPX, STY);
    DEF_INS(0x8C, 4, ABS, STY);

    DEF_INS(0xAA, 2, IMP, TAX);
    DEF_INS(0xA8, 2, IMP, TAY);
    DEF_INS(0xBA, 2, IMP, TSX);
    DEF_INS(0x8A, 2, IMP, TXA);
    DEF_INS(0x9A, 2, IMP, TXS);
    DEF_INS(0x98, 2, IMP, TYA);

#undef DEF_INS
#undef INS
}
