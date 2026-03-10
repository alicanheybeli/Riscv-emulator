cmake_minimum_required(VERSION 3.10)
project(RISCV_Emulator VERSION 1.0 LANGUAGES CXX)

# 1. Standard Enforcement
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Disables non-standard compiler-specific features

# 2. Strict Warning Flags (GCC/Clang)
# These flags mimic many MISRA C++:2008 / AUTOSAR rules
set(STRICT_FLAGS
    -Wall               # Essential warnings
    -Wextra              # Extra useful warnings,
    -Werror              # Treat ALL warnings as errors (Crucial for MISRA)
    -Wpedantic           # Strict ISO C++ compliance
    -Wshadow             # Warn if a variable shadows another
    -Wnon-virtual-dtor   # Ensure proper inheritance cleanup
    -Wold-style-cast     # Forbid C-style casts (e.g., (int)x)
    -Wcast-align         # Warn on pointer casts that increase alignment
    -Wunused             # Warn on anything unused
    -Woverloaded-virtual # Warn if you accidentally hide a virtual function
    -Wconversion         # Warn on type conversions that lose data (e.g., double to int)
    -Wsign-conversion    # Warn on signed/unsigned mismatches
    -Wnull-dereference   # Detect potential null pointer usage
    -Wdouble-promotion   # Warn if float is implicitly promoted to double
    -Wformat=2           # Enhanced printf/scanf checking
)

# 3. Project Structure
include_directories(include)

set(SOURCES
    src/main.cpp
    
    src/memory.cpp

)

add_executable(${PROJECT_NAME} ${SOURCES})

# 4. Apply the flags to the target
target_compile_options(${PROJECT_NAME} PRIVATE ${STRICT_FLAGS})