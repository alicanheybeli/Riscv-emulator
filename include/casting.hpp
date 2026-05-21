#pragma once
#include <cstdint>
#include <type_traits>
#include <concepts>

namespace cast {
    template<typename T>
    concept Numerical = std::is_arithmetic_v<T>;
    constexpr uint32_t u32(Numerical auto val) { return static_cast<uint32_t>(val); }
    constexpr int32_t  s32(Numerical auto val) { return static_cast<int32_t>(val); }
    constexpr uint16_t u16(Numerical auto val) { return static_cast<uint16_t>(val & 0xFFFF); }
    constexpr uint8_t u8(Numerical auto val) { return static_cast<uint8_t>(val & 0xFF); }
    constexpr int16_t s16(Numerical auto val) { return static_cast<int16_t>(val & 0xFFFF); }
    constexpr int8_t s8(Numerical auto val) { return static_cast<int8_t>(val & 0xFF); }
    constexpr uint32_t zeroextend(uint16_t val)
    {   
        return cast::u32(val) & 0xFFFF;        
    }
    constexpr uint32_t zeroextend(uint8_t val)
    {
        return cast::u32(val) & 0xFF;
    }

    constexpr uint32_t signextend(uint16_t val)
    {
        return cast::u32(cast::s32(cast::s16(val)));
    }
    constexpr uint32_t signextend(uint8_t val)
    {
        return cast::u32(cast::s32(cast::s8(val)));
    }
}
