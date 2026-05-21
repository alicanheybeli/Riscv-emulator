#include "memory.hpp"


void Memory::LoadProgram(const uint8_t* binary, size_t size) {
        if (size > ramsize) {
            std::cout << "program size overflow:" << size;
            return;
        }
        std::memcpy(RAM, binary, size);
    }
uint32_t Memory::read32(uint32_t address){
    return cast::u32(read8(address))    | 
     (cast::u32(read8(address+1))<< 8)  |
     (cast::u32(read8(address+2))<< 16) |
     (cast::u32(read8(address+3))<< 24);
}
uint16_t Memory::read16(uint32_t address){
    return cast::u16(read8(address))    | 
     (cast::u16(read8(address+1))<< 8)  ;
}
uint8_t Memory::read8(uint32_t address)
{
    if (address < baseaddress || address >= ramsize + baseaddress)
    {
        std::cout << "attempted to reach to adress outside range:" << address;
        return 0;
    }
    
    return RAM[address - baseaddress];
};
void Memory::write16(uint32_t address,uint32_t value)
{
    write8(address,    cast::u8(extractbits(8,0,value)));
    write8(address+1,  cast::u8(extractbits(8,8,value)));

};
void Memory::write32(uint32_t address,uint32_t value)
{
    write8(address,    cast::u8(extractbits(8,0,value)));
    write8(address+1,  cast::u8(extractbits(8,8,value)));
    write8(address+2,  cast::u8(extractbits(8,16,value)));
    write8(address+3,  cast::u8(extractbits(8,24,value)));
};
void Memory::write8(uint32_t address,uint8_t value)
{
    if (address < baseaddress || address >= ramsize + baseaddress)
    {
        std::cout << "attempted to reach to adress outside range:" << address;
    }
    RAM[address] = value;
};