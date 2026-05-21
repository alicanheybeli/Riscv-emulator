# RISC-V Emulator

A RISC-V RV32I instruction set emulator written in C++20. Work in progress.

## What it does

- Implements a fetch-decode-execute pipeline
- Decodes all RV32I instruction formats: R, I, S, B, J, U
- Executes most base integer instructions
- Little-endian memory model with base address at `0x80000000`
- 1MB RAM
- Type-safe casting utilities and bit manipulation helpers

## Implemented instructions

| Type | Instructions |
|------|-------------|
| R | ADD, SUB, AND, OR, XOR |
| I | ADDI, ANDI, ORI, XORI, SLLI |
| Load | LB, LH, LW, LBU, LHU |
| Store | SB, SH, SW |
| Branch | BEQ, BNE, BLT, BGE, BLTU, BGEU |
| Jump | JAL, JALR |
| Upper | LUI, AUIPC |

## Not yet implemented

- EBREAK/ECALL halt execution
- SLT, SLTU, SRL, SRA
- ELF binary loading (stub only)

## Build

```
cmake -B build
cmake --build build
```

Requires a C++20 compiler (GCC or Clang).

## Project structure

```
src/
  main.cpp       — entry point
  cpu.cpp        — processor implementation
  memory.cpp     — memory read/write
include/
  cpu.hpp        — processor and instruction classes
  memory.hpp     — memory subsystem
  casting.hpp    — type-safe cast utilities
  bitmasking.hpp — bit extraction helpers
```
