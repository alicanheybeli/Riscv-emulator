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
}
