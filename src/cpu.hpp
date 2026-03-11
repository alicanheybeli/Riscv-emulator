    #include <cstdint>
    #include <iostream>
    #include <stdexcept>
    #include "memory.hpp"
    #include "bitmasking.hpp"
    #include "casting.hpp"
    const uint32_t test_program[] = {
        0x00a00093, // ADDI x1, x0, 10
        0x01400113, // ADDI x2, x0, 20
        0x002081b3, // ADD  x3, x1, x2
        0x00000073  // EBREAK
    };

    enum Funct3_ALU {
        F3_ADD_SUB = 0x0,
        F3_SLL     = 0x1,
        F3_SLT     = 0x2,
        F3_OR      = 0x6,
        F3_AND     = 0x7
    };

    enum class OpcodeTypes:uint8_t
    {
        OP_REG = 0x33, //0110011 operations that take in 2 registers and output to a register
        OP_IMM = 0x13, //0010011 Immideate instructions
        ENV = 0x73 //1110011 enviroment instructions
    };

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
        OpcodeTypes getOpcode() const { return static_cast<OpcodeTypes>(opcode); }
        uint8_t getRd() const {return (rd);}
        Funct3_ALU getFunct3() const {return static_cast<Funct3_ALU>(funct3);}
        uint8_t getRS1()    const { return rs1; }
        uint8_t getRS2() const {return rs2;}
        uint8_t getFunct7() const {return funct7;}
        int32_t getImm()    const { return imm; }

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
        switch (opcode)
        {
        case 0x33: // R types
        {
            rs2 = cast::u8((inst >> 20) & 0x1Fu);
            funct7 = cast::u8((inst >> 25) & 0x7Fu);
            break;
        }
        case 0x13: // I types
        case 0x03: // Load instructions. we can get away with using the same decode here. the top 20 bits become the imm field.
        {
            uint32_t raw_imm = ((inst >> 20) & 0xFFF); 
            if (raw_imm & 0x800) {
                raw_imm |= 0xFFFFF000;
            }
            imm = cast::s32(raw_imm);
            break;
        }
        case 0x23: //S-type
        {
            rs2 = (inst >> 20) & 0x1F;
            uint32_t top7 = cast::u32(inst >> 25) & 0x7F; //imm[11:5]
            uint32_t bottom5 = (inst >> 7) & 0x1F; //imm[4:0]
            //reassemble the immideate
            uint32_t full_imm = (top7 << 5) | bottom5;
            if (full_imm & 0x800) { // If bit 11 is set
                full_imm |= 0xFFFFF800; // Sign extend to 32 bits
            }  
            imm = cast::s32(full_imm);
            break;
        }
        case 0x73:
            imm = (cast::s32(inst >> 20));
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
        uint32_t sp = 0;
        uint32_t registers[32];
        Memory* memory;
        bool is_running = true;
    public:

        void PrintState(){
            std::cout << " | pc: " << pc << " | sp: " << sp;
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
            std::cin.get();
            decodedinstruction.PrintInstruction();
            PrintState();
            
        }
        inline void RTypesExecute( [[maybe_unused]]Instruction instruction)
        {
            switch (instruction.getFunct3())
            {
            case Funct3_ALU::F3_ADD_SUB: //addi
            {
                registers[instruction.getRd()] = registers[instruction.getRS1()] + registers[instruction.getRS2()];
                break;
            }
            default:
                break;
            }
        }
        inline void ITypesExecute(Instruction instruction)
        {
            switch (instruction.getFunct3())
            {
            case Funct3_ALU::F3_ADD_SUB: //addi
            {
                registers[instruction.getRd()] = cast::u32(cast::s32(registers[instruction.getRS1()]) + cast::s32(instruction.getImm()));
                break;
            }
            default:
                break;
            }
        }
        inline void ENVTypesExecute(Instruction instruction)
        {
            switch (instruction.getImm())
            {
            case 1U: //ebreak
                /* code */
                break;
            
            default:
                break;
            }
        }
        void Execute(Instruction instruction)
        {

        
            switch (instruction.getOpcode())
            {
            case OpcodeTypes::OP_REG:
                RTypesExecute(instruction);
                
                break;
            case OpcodeTypes::OP_IMM:
                ITypesExecute(instruction);
                break;
            case OpcodeTypes::ENV:
                ENVTypesExecute(instruction);
                break;
            default:
                break;
            }
            if (true/* not branch/jump */)
            {
                pc += 4;
            }
            

            
        }

    };

    Processor::Processor(/* args */)
    {
        for(int i=0; i<32; i++) registers[i] = 0;
    }

    Processor::~Processor()
    {
    }
