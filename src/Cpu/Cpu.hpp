#ifndef CPU_HPP
#define CPU_HPP

#include "../Types.hpp"
#include <array>
#include <functional>
class Ram;

class Cpu
{
public:
    Cpu(Ram &ram);
    ~Cpu();

    void Clock();

private:
    
    
    // R/W memory
    Word Read(DWord address);
    Word Write(DWord address, Word value);
    
    // Push 8 bit value onto the stack
    Word PushWord(Word value);
    // Fetch and delete the 8 bit value on the top of the stack
    Word PopWord();

    // Fetch and delete the 16 bit value on the top of the stack
    DWord PushDWord(DWord value);
    // Fetch and delete the 16 bit value on the top of the stack
    DWord PopDWord();

    // Registers

    // Program counter: pointer the executed instruction
    DWord m_PC;
    // Stack pointer: offset between the beggining and the top of the stack
    Word m_SP;
    // A Register: used for all arithmetic and logic instructions
    Word m_A;
    // X Register: commonly used to store offsets and counters
    // and can be used to R/W the stack pointer
    Word m_X;
    // Y Register: similar to the X register, commonly used
    // to store offsets and counters and has no special funcitonnalities
    Word m_Y;

    union Status {
        Word value;
        struct
        {
            // Carry flag: contains the overflowed bit caused by
            // an arithmetic operation
            bool C : 1;
            // Zero flag: indicates if the result of an
            // arithmetic operation returns a null result
            bool Z : 1;
        	// Disable interrupt: while this flag is set the cpu
            // ignores all interrupt request (except for NMI)
            bool I : 1;
            // Decimal mode: unsupported by the nes
            bool D : 1;
            // Break command: set when the BRK instruction
            // has been executed
            bool B : 1;
            // Unused flag
            bool U : 1;
            // Overflow flag: indicates if the result of an
            // arithmetic operation overflows the 8 bit limit
            bool V : 1;
            // Negative flag: indicates if the result of an
            // arithmetic operation returns a negative result
            bool N : 1;
        };
    } m_Status;

    Ram &m_Ram;

    /*
       Remaining skipped cycles

       Each instruction have a variable amount of cycles therefore to
       respect cycle accuracy we have to wait until the remaining cycles
       are completed
     */
    QWord m_RemainingCycles;

    // The stack memory begins at the 256th byte (second page)
    static constexpr DWord s_StackBase = 0x0100;

    // Interrupt vectors: location of interruption microprograms

    static constexpr DWord s_IrqVector = 0xFFFE;
    static constexpr DWord s_ResetVector = 0xFFFC;
    static constexpr DWord s_NmiVector = 0xFFFA;

    void Interrupt(DWord interruptVector);
    // Interrupt request
    void IRQ();
    // Non maskable interrupt request
    void NMI();

    void Reset();

    /*
       Represents an implicit data source

       Whenever the address is equal to 0xFFFF we don't fetch the value from
       the ram but from the accumulator
     */
    DWord s_ImplicitSource = 0xFFFF;

    /*
       Implicit addressing mode
       We return 0xFFFF, it means that if we want to fetch
       data (which is not always the case) we have to return
       the accumulator register
     */
    DWord IMP();
    /*
       Immediate addressing mode
       Using the specified value directly without any ram fetching
     */
    DWord IMM();
    /*
        Relative addrssing mode
    Offset the program counter by the specified value
    Returns the shifted program counter
    */
    DWord REL();

    /*
       Zero page addressing mode utility
       Offsets and wraps the specified value to be zero paged
     */
    DWord ZeroPage(DWord offset);
    /*
       Zero page addressing mode
       The data is meant to be located within the first ram page
     */
    DWord ZER();
    // Zero page addressing mode with x offset
    DWord ZPX();
    // Zero page addressing mode with y offset
    DWord ZPY();

    // Absolute addressing mode utility
    DWord Absolute(DWord offset);
    /*
      Absolute addressing mode
      The specified value directly contains the address to the fetched value
     */
    DWord ABS();
    // Absolute addressing mode with x offset
    DWord ABX();
    // Absolute addressing mode with y offset
    DWord ABY();

    /*
      Indirect addressing mode
      The value contains a 16-bit address to the fetched data pointer
     */
    DWord IND();
    /*
      Indexed Indirect addressing mode
      The pointer is shifted by the one paged X register
     */
    DWord IDX();
    // Indirect indexed addressing mode
    DWord IDY();

    struct Instruction
    {
        Word cycles;
        std::function<DWord(void)> addressing;
        std::function<void(DWord)> operation;
    };

    std::array<Instruction, 256> m_InstructionSet;
    void GenerateInstructionSet();

    // Operation utilities
    // Fetch a word during an operation, this operation checks if the source is implicit
    Word FetchWord(DWord source);
    // Set a word during an operation, this operation checks if the source is implicit
    Word SetWord(DWord address, Word value);
    Word Addition(DWord operand);
    void Branch(bool condition, DWord destination);
    void Compare(Word reg, Word operand);
    Word Increment(Word operand, bool positive);
    void LoadRegister(Word &reg, Word value);
    void Return(Word offset);
    void Transfer(Word &from, Word &to);

    // Operations
    // Add with carry bit
    void ADC(DWord source);
    // Bitwise and
    void AND(DWord source);
    // Arithmetic shift left
    void ASL(DWord source);
    // Branch if carry clear
    void BCC(DWord destination);
    // Branch if carry set
    void BCS(DWord destination);
    // Branch if equal
    void BEQ(DWord destination);
    // Bit test
    void BIT(DWord source);
    // Branch if minus
    void BMI(DWord destination);
    // Branch if not equal
    void BNE(DWord destination);
    // Branch if positive
    void BPL(DWord destination);
    // Break
    void BRK(DWord);
    // Branch if overflow clear
    void BVC(DWord destination);
    // Branch if overflow set
    void BVS(DWord destination);
    // Clear carry bit
    void CLC(DWord);
    // Clear decimal bit
    void CLD(DWord);
    // Clear disable interrupt bit
    void CLI(DWord);
    // Clear the overflow flag
    void CLV(DWord);
    // Compare
    void CMP(DWord source);
    // Compare X register
    void CPX(DWord source);
    // Compare Y register
    void CPY(DWord source);
    // Decrement
    void DEC(DWord source);
    // Decrement Y
    void DEX(DWord);
    // Decrement X
    void DEY(DWord);
    // Bitwise exclusive or
    void EOR(DWord source);
    // Increment
    void INC(DWord source);
    // Increment x register
    void INX(DWord source);
    // Increment y register
    void INY(DWord source);
    // Jump
    void JMP(DWord destination);
    // Jump to subroutine
    void JSR(DWord destination);
    // Load the accumulator
    void LDA(DWord source);
    // Load the x register
    void LDX(DWord source);
    // Load the x register
    void LDY(DWord source);
    // Bitwise shift right
    void LSR(DWord source);
    // No operation
    void NOP(DWord);
    // Bitwise or
    void ORA(DWord source);
    // Push the accumulator onto the stack
    void PHA(DWord);
    // Push the processor status onto the stack
    void PHP(DWord);
    // Pull the accumulator from the stack
    void PLA(DWord);
    // Pull the processor status from the stack
    void PLP(DWord);
    // Bitwise rotate left
    void ROL(DWord source);
    // Bitwise rotate right
    void ROR(DWord source);
    // Return from interrupt
    void RTI(DWord);
    // Return from subroutine
    void RTS(DWord);
    // Substract with carry bit
    void SBC(DWord source);
    // Set the carry bit
    void SEC(DWord);
    // Set the decimal bit
    void SED(DWord);
    // Set the disable interrupt flag
    void SEI(DWord);
    // Store the accumulator
    void STA(DWord source);
    // Store the x register
    void STX(DWord source);
    // Store the y register
    void STY(DWord source);
    // Transfer the accumulator to the x register
    void TAX(DWord);
    // Transfer the accumulator to the y register
    void TAY(DWord);
    // Transfer the stack pointer to the x register
    void TSX(DWord);
    // Transfer the x register to the accumulator
    void TXA(DWord);
    // Transfer the x register to the stack pointer
    void TXS(DWord);
    // Transfer the x register to the accumulator
    void TYA(DWord);
    // Illegal operation
    void ILL(DWord);
};

#endif
