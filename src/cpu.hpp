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
    OR      = 0x6,
    XOR     = 0x4,
    AND     = 0x7
};
enum Funct3_STR {
    BYTE = 0x0,
    HALF = 0x1,
    WORD = 0x2

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
    OP_JMPI = 0x6F,
    OP_JMPR = 0x68,
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
    OpcodeTypes getOpcode() const { return static_cast<OpcodeTypes>(opcode); }
    uint32_t getRd() const {return (rd);}
    Funct3_ALU getFunct3_ALU() const {return static_cast<Funct3_ALU>(funct3);}
    Funct3_CMP getFunct3_CMP() const {return static_cast<Funct3_CMP>(funct3);}
    Funct3_STR getFunct3_STR() const {return static_cast<Funct3_STR>(funct3);}
    uint32_t getRS1()    const { return rs1; }
    uint32_t getRS2() const {return rs2;}
    uint32_t getFunct7() const {return funct7;}
    int32_t getImmS()    const { return imm; }
    uint32_t getImmU()    const { return cast::u32(imm);}
    Instruction(uint32_t instruction);
    ~Instruction();
    void PrintInstruction(void){
        std::cout << "opcode: 0x" << std::hex<<  cast::u32(opcode) 
        << " | rd: " << std::dec << cast::u32(rd)
        << " | funct3: " << cast::u32(funct3)
        << " | rs1: " << cast::u32(rs1); 
        
    switch (opcode)
    {
    case 0x33: // R types
        std::cout << " | rs2: "<< cast::u32(rs2) 
        << "| funct7: " << cast::u32(funct7);
        break;
    case 0x13: // I types
    case 0x03: // Load instructions.
        std::cout << " | imm: "<< imm; 
        break;
    case 0x23: //S-type
        std::cout << " | rs2: "<<  cast::u32(rs2) << " | imm: "<< imm;
        break;
    case 0x73: 
        std::cout << " | imm: "<< imm; 
        break;
    default:
        std::cout << "\n instruction printing is not implemented. " ;
        break;
    }
    }
};
Instruction::Instruction(uint32_t inst)
{
    opcode = inst & 0x7F; //7 bits
    rd = (inst >> 7) & 0x1F; //5 bits
    funct3 = (inst >> 12) & 0x07; // 3 bits
    rs1 = (inst >> 15) & 0x1F;
    
    rs2 = 0;
    funct7 = 0;
    imm = 0;
    // we need to decode based on opcode because opcodes can change how an instruction is decoded. 
    switch (getOpcode())
    {
    case OpcodeTypes::OP_REG: // R types
    {
        rs2 = cast::u8((inst >> 20) & 0x1Fu);
        funct7 = cast::u8((inst >> 25) & 0x7Fu);
        break;
    }
    case OpcodeTypes::ENV:
    case OpcodeTypes::OP_JMPI:
    case OpcodeTypes::OP_IMM: // I types
    case OpcodeTypes::OP_LD: // Load instructions. we can get away with using the same decode here. the top 20 bits become the imm field.
    {
        uint32_t raw_imm = ((inst >> 20) & 0xFFF); 
        if (raw_imm & 0x800) {
            raw_imm |= 0xFFFFF000;
        }
        imm = cast::s32(raw_imm);
        break;
    }
    case OpcodeTypes::OP_STR: //S-type
    {
        rs2 = (inst >> 20) & 0x1F;
        uint32_t top7 = cast::u32(inst >> 25) & 0x7F; //imm[11:5]
        uint32_t bottom5 = (inst >> 7) & 0x1F; //imm[4:0]
        //reassemble the immideate
        uint32_t full_imm = (top7 << 5) | bottom5;
        if (full_imm & 0x800) { // If bit 12 is set
            full_imm |= 0xFFFFF800; // Sign extend to 32 bits
        }  
        imm = cast::s32(full_imm);  
        break;
    }

    case OpcodeTypes::OP_JMPR:
        {uint32_t raw_imm = 0;
        raw_imm |= ((inst >> 31) & 0x1) << 20; // Bit 20
        raw_imm |= ((inst >> 12) & 0xFF) << 12; // Bits 19:12
        raw_imm |= ((inst >> 20) & 0x1) << 11;  // Bit 11
        raw_imm |= ((inst >> 21) & 0x3FF) << 1; // Bits 10:1

        // Final step: Sign-extend from bit 20 to 31
        // If bit 20 is 1, fill the top bits with 1s
        if (raw_imm & 0x100000) {
            raw_imm |= 0xFFE00000;
        }
        imm = cast::s32(raw_imm);   
    }
        break;
    case OpcodeTypes::OP_BRNCH:
    {
        uint32_t raw_imm = 0;
        raw_imm |= ((inst >> 31) & 0x1) << 12; // Bit 12
        raw_imm |= ((inst >> 7)  & 0x1) << 11; // Bit 11
        raw_imm |= ((inst >> 25) & 0x3F) << 5; // Bits 10:5
        raw_imm |= ((inst >> 8)  & 0xF) << 1;  // Bits 4:1

        // Final step: Sign-extend from bit 12 to 31
        if (raw_imm & 0x1000) {
            raw_imm |= 0xFFFFE000;
        }
        imm = cast::s32(raw_imm);
        break;
    }
    /*case OpcodeTypes::ENV:
        imm = (cast::s32(inst >> 20));
        break;*/
    case OpcodeTypes::OP_UP:
    case OpcodeTypes::OP_UP2:
        // U-type: Extract bits 31 to 12
        imm = cast::s32(inst & 0xFFFFF000);     
    break;
    default:
    {
        std::cout << "\n instruction type is not implemented." ;
        break;
    }
}
}
Instruction::~Instruction()
{
}
class Processor
{
private:
    uint32_t pc = baseaddress;
    uint32_t registers[32];
    Memory* memory;
    bool is_running = true;
public:
    inline uint32_t getregisterU(uint32_t r)
    {
        if (r == 0)
        {
            return 0;
        }
        return registers[r];
    }
    int32_t getregisterS(uint32_t r)
    {
        return cast::s32(getregisterU(r));
    }
    void setregister(uint32_t r, uint32_t value)
    {
        registers[r] = value;
    }
    void PrintState(){
        std::cout << " | pc: " << pc;
        /*for (size_t i = 0; i <= 31; i++)
        {
            std::cout << "r" << i << ":" << std::hex << registers[i];
        }*/ 
    }
    Processor(/* args */);
    ~Processor();
    void run(Memory* program){
        memory = program;
        while (is_running)
        {
            Step();
        }
    }
    void Step()
    {
        uint32_t fetchedinstruction = memory->read32(pc);
        Instruction decodedinstruction = Instruction(fetchedinstruction);
        Execute(decodedinstruction);
        //std::cin.get();
        //decodedinstruction.PrintInstruction();
        //PrintState();
        
    }
    inline void RTypesExecute(Instruction instruction)
    {
        auto rd = instruction.getRd();
        auto rs1 = registers[instruction.getRS1()];
        auto rs2 = registers[instruction.getRS2()];
        switch (instruction.getFunct3_ALU())
        {
        case Funct3_ALU::ADD_SUB: //addi
        {
            switch (instruction.getFunct7())
            {
            case 0x00:
                registers[rd] = rs1 + rs2;
                break;
            
            case 0x20:
                registers[rd] = rs1 - rs2;
                break;
            
            default:
                break;
            }
            
            break;
        }
        case Funct3_ALU::AND:
        {
            registers[rd] = rs1 & rs2;
            break;
        }
        case Funct3_ALU::OR:
        {
            registers[rd] = rs1 | rs2;
            break;
        }
        case Funct3_ALU::XOR:
            registers[rd] = rs1 ^ rs2;
            break;
        default:
            break;
        }
    }
    inline void ITypesExecute(Instruction instruction)
    {
        auto rd = instruction.getRd();
        auto rs1 = getregisterU(instruction.getRS1());

        //auto rs2 = registers[instruction.getRS2()];
        //auto imm_s = instruction.getImmS();
        auto imm_u = instruction.getImmU();
        switch (instruction.getFunct3_ALU())
        {
        case Funct3_ALU::ADD_SUB: //addi
        {
            registers[rd] = rs1 + imm_u;
            break;
        }
        case Funct3_ALU::AND:
        {
            registers[rd] = rs1 & imm_u;
            break;
        }
        case Funct3_ALU::OR:
        {
            registers[rd] = rs1 | imm_u;
            break;
        }
        case Funct3_ALU::XOR:
            registers[rd] = rs1 ^ imm_u;
            break;
        case Funct3_ALU::SLL:
        {
            registers[rd] = rs1 << extractbits(5,0,cast::u32(imm_u));
            break;
        }
        default:
            break;
        }
    }
    inline uint32_t BTypesExecute(Instruction instruction)
    {   

        //auto rd = instruction.getRd();
        auto rs1 = getregisterU(instruction.getRS1());
        auto rs2 = getregisterU(instruction.getRS2());
        //auto imm_s = instruction.getImmS();
        auto imm_u = instruction.getImmU();
        switch (instruction.getFunct3_CMP())
        {
        case Funct3_CMP::EQUAL:
            if (rs1 == rs2)
            {
                return pc + imm_u;
            }
            
            break;
        case Funct3_CMP::NOTEQUAL:
            if (rs1  !=  rs2)
            {
                return pc + imm_u;
            }
            break;
        case Funct3_CMP::LESSTHAN:
            if (cast::s32(rs1)  <  cast::s32(rs2))
            {
                return pc + imm_u;
            }
            break;
        case Funct3_CMP::EQUALORMORE:
            if (cast::s32(rs1) >=  cast::s32(rs2))
            {
                return pc + imm_u;
            }
            break;
        case Funct3_CMP::LESSTHAN_U:
            if (rs1  <  rs2)
            {
                return pc + imm_u;
            }
        
            break;
        case Funct3_CMP::EQUALORMORE_U:
            if (rs1  >=  rs2)
            {
                return pc + imm_u;
            }
            
        default:
            break;
        }
        return pc + 4;
    }
    inline void STypesExecute(Instruction instruction)
    {
        
        //auto rd = instruction.getRd();
        auto rs1 = getregisterU(instruction.getRS1());
        auto rs2 = getregisterU(instruction.getRS2());
        //auto imm_s = instruction.getImmS();
        auto imm_u = instruction.getImmU();
        switch (instruction.getFunct3_STR())
        {
        case Funct3_STR::BYTE: 
            memory->write8(rs1+imm_u, cast::u8(extractbits(8,0,rs2)));
            break;
        case Funct3_STR::HALF: 
            memory->write16(rs1+imm_u, cast::u16(extractbits(16,0,rs2)));
            break;
        case Funct3_STR::WORD: 
            memory->write32(rs1+imm_u, cast::u32(rs2));
            break;
        
        default:
            break;
        }
    }
    inline void ENVTypesExecute(Instruction instruction)
    {
        switch (instruction.getImmS())
        {
        case 1U: //ebreak
            /* code */
            break;
        
        default:
            break;
        }
    }
    inline uint32_t JTypesExecute(Instruction instruction)
    {
        auto rd = instruction.getRd();
        auto rs1 = getregisterU(instruction.getRS1());
        auto imm_u = instruction.getImmU();
        switch (instruction.getOpcode())
        {
        case OpcodeTypes::OP_JMPI: 
            registers[rd] = pc+4;
            return (pc + imm_u);
            break;
        case OpcodeTypes::OP_JMPR:
            registers[rd] = pc+4;
            return (pc + imm_u + rs1);
            break;
        default:
            break;
        }
        return pc+4;
    }
    void Execute(Instruction instruction)
    {
        auto nextpc = pc +4;

        switch (instruction.getOpcode())
        {
        case OpcodeTypes::OP_REG:
            RTypesExecute(instruction);
            break;
        case OpcodeTypes::OP_IMM:
            ITypesExecute(instruction);
            break;
        case OpcodeTypes::OP_BRNCH:
            nextpc = BTypesExecute(instruction);
            break;
        case OpcodeTypes::OP_JMPI:
        case OpcodeTypes::OP_JMPR:
            nextpc = JTypesExecute(instruction);
            break;
        case OpcodeTypes::OP_STR:
            STypesExecute(instruction);
            break;
        case OpcodeTypes::ENV:
            ENVTypesExecute(instruction);
            break;

        default:
            break;
        }

        pc = nextpc;

        
        
    }
};
Processor::Processor(/* args */)
{
    for(int i=0; i<32; i++) registers[i] = 0;
}
Processor::~Processor()
{
}