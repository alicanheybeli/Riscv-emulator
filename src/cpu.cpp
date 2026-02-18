#include <cstdint>

class Instruction
{
private:
    uint8_t opcode;//opcode bits 0-6
    uint8_t rd; //destination register, bits 7-11
    uint8_t funct3; // used to distingiush instructions with the same opcode. 12-14
    uint8_t rs1; //source register 1 15-19
    uint8_t rs2; //source register 2 20-24
    uint8_t funct7; // distinguishes R-type instructions 25-31
    int32_t imm; //immediate value

public:
    Instruction(uint32_t instruction);
    ~Instruction();
};

Instruction::Instruction(uint32_t inst)
{
    opcode = inst & 0b1111111; //7 bits
    rd = (inst >> 7) & 0b11111; //5 bits
    funct3 = (inst >> 12) & 0b111; // 3 bits
    rs1 = (inst >> 15) & 0b11111;
    
    rs2 = 0;
    funct7 = 0;
    imm = 0;


    // we need to decode based on opcode because opcodes can change how an instruction is decoded. 
    switch (opcode)
    {
    case 0x33: // R types
        rs2 = (inst >> 20) & 0x1F;
        funct7 = (inst >> 25) & 0x7F;
        break;
    case 0x13: // I types
    case 0x03: // Load instructions. we can get away with using the same decode here. the top 20 bits become the imm field.
        imm = ((int32_t)inst >> 20); // we need to turn it into an int before shifting so that it does an arithmetic shift instead of a logical one. 
        break;
    case 0x23: //S-type
        rs2 = (inst >> 20) & 0x1F;
        int32_t top7 = ((int32_t)inst >> 25);
        int32_t bottom5 = (inst >> 7) & 0x1F;
        //reassemble the immideate
        imm = (top7 << 5) | bottom5;
        break;
    default:
        
        break;
    }
}

Instruction::~Instruction()
{
}



class cpu
{
private:
    uint32_t pc;
    uint32_t sp;
    uint32_t registers[32];
    uint8_t memory[1024];
public:

    cpu(/* args */);
    ~cpu();
    void step()
    {

    }
    int fetch()
    {

    }
    Instruction decode()
    {

    }
    void execute()
    {

    }
};

cpu::cpu(/* args */)
{
}

cpu::~cpu()
{
}
