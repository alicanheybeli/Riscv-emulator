cmake_minimum_required(VERSION 3.10)

project(RiscV_Emulator 
        VERSION 1.0 
        DESCRIPTION "RV32I Emulator"
        LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-*,bugprone-*,performance-*,portability-*,modernize-*,readability-identifier-naming")

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#include_directories(include)

set(SOURCES 
    src/main.cpp 
    src/cpu.cpp 
    src/memory.cpp
)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
add_executable(${PROJECT_NAME} ${SOURCES})