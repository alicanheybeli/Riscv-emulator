#include <cstdint>
#include <stdexcept>
#pragma once 




inline constexpr uint32_t bitmask(uint8_t bitamount)
{
    if (bitamount > 32) throw std::out_of_range("bitamount > 32");
    if (bitamount == 32) return 0xFFFFFFFF;
    return (1U << bitamount)-1;
}

inline constexpr uint32_t extractbits(uint8_t bitamount, uint8_t offset,uint32_t value)
{
    if (offset + bitamount > 32) { throw std::out_of_range("Extract range exceeds 32 bits");
    }
    return  (value >> offset) & bitmask(bitamount);
}// if I have to write another hex I swear to god.
