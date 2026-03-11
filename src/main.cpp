#include <iostream>
#include "cpu.hpp"



int main()
{
    std::cout << "RiscV Emulator\n";
    
    Memory* memory = new Memory();
    
    memory->LoadProgram(reinterpret_cast<const uint8_t*>(test_program) ,16);
    auto cpu = Processor();
    cpu.run(memory);

    return 0;
}
