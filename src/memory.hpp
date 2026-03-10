#include <cstdint>
#include <iostream>
#include <cstring>
#include "bitmasking.hpp"
#pragma once 
constexpr uint32_t ramsize = 1024*4;
class Memory
{
private:
    uint8_t RAM[ramsize];
public:
    uint32_t read32(uint32_t address);
    uint8_t read8(uint32_t address);
    void write32(uint32_t address,uint32_t value);
    void write8(uint32_t address,uint8_t value);
    void LoadProgram(const uint8_t* binary, size_t size);
};