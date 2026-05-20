#pragma once
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include "memory.hpp"
#include "bitmasking.hpp"
#include "casting.hpp"

const uint32_t test_program[] = {
    0x00a00093, // ADDI x1, x0, 10
    0x01400113, // ADDI x2, x0, 20
    0xffb08213, // ADDI x4, x1, -5    (x4 = 10 - 5 = 5)
    0x002081b3, // ADD  x3, x1, x2
    0x00000073  // EBREAK
};


enum Funct3_ALU {
    ADD_SUB = 0x0,
    SLL     = 0x1,
    SLT     = 0x2,
    SLT_U   = 0x3,
    OR      = 0x6,
    XOR     = 0x4,
    SRL     = 0x5,
    AND     = 0x7
};
enum Funct3_STR_LD {
    BYTE = 0x0,
    HALF = 0x1,
    WORD = 0x2,
    BYTE_U = 0x4,//zero-extends
    HALF_U = 0x5//zero-extends
};
enum Funct3_CMP
{
    EQUAL = 0x0,
    NOTEQUAL = 0x1,
    LESSTHAN = 0x4,
    EQUALORMORE = 0x5,
    LESSTHAN_U = 0x6, //U funct3's zero extend.
    EQUALORMORE_U = 0x7,
};
enum class OpcodeTypes:uint8_t
{
    OP_REG = 0x33, //0110011 operations that take in 2 registers and output to a register
    OP_IMM = 0x13, //0010011 Immideate instructions
    ENV = 0x73, //1110011 enviroment instructions
    OP_BRNCH = 0x63,
    OP_JMP_I = 0x67,
    OP_JMP = 0x6F,
    OP_LD =0x03, 
    OP_STR = 0x23,
    OP_UP = 0x37,
    OP_UP2= 0x17
    
};
class Instruction
{
private:
    uint32_t opcode;//opcode bits 0-6
    uint32_t rd; //destination register, bits 7-11
    uint32_t funct3; // used to distingiush instructions with the same opcode. 12-14
    uint32_t rs1; //source register 1 15-19
    uint32_t rs2; //source register 2 20-24
    uint32_t funct7; // distinguishes R-type instructions 25-31
    int32_t imm; //immediate value
public:
    OpcodeTypes getOpcode() const;
    uint32_t getRd() const;
    Funct3_ALU getFunct3_ALU() const;
    Funct3_CMP getFunct3_CMP() const;
    Funct3_STR_LD getFunct3_STR_LD() const;
    uint32_t getRS1()    const ;
    uint32_t getRS2() const;
    uint32_t getFunct7() const;
    int32_t getImmS()    const;
    uint32_t getImmU()    const ;
    Instruction(uint32_t instruction);
    ~Instruction();
    void PrintInstruction(void);
};

class Processor
{
private:
    uint32_t pc = baseaddress;
    uint32_t registers[32];
    Memory* memory;
    bool is_running = true;
public:
    inline uint32_t getregisterU(uint32_t r);
    int32_t getregisterS(uint32_t r);
    void setregister(uint32_t r, uint32_t value);
    void PrintState();
    Processor(/* args */);
    ~Processor();
    void run(Memory* program);
    void Step();
    inline void RTypesExecute(Instruction instruction);
    inline void ITypesExecute(Instruction instruction);
    inline uint32_t BTypesExecute(Instruction instruction);
    inline void STypesExecute(Instruction instruction);
    inline void ENVTypesExecute(Instruction instruction);
    inline uint32_t JTypesExecute(Instruction instruction);
    inline void LTypesExecute(Instruction instruction);
    void Execute(Instruction instruction);
};
