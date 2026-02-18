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
    uint32_t imm; //immediate value

public:
    Instruction(uint32_t instruction);
    ~Instruction();
};

Instruction::Instruction(uint32_t inst)
{
    opcode = inst & 0b1111111; //7 bits
    rd = (inst >> 7) & 0b11111; //5 bits
    funct3 = (inst >> 12) & 0b111; // 3 bits
    rs1 = (inst >> 15) & 0b1111;
    
    rs2 = 0;
    funct3 = 0;
    imm = 0;

    switch ()
    {
    case /* constant-expression */:
        /* code */
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
