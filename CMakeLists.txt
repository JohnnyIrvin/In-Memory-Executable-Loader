cmake_minimum_required(VERSION 3.25)
project(in-memory-executable-loader)

# Set compiler flags
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# Set build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set source files
set(SOURCES src/main.c src/pe_loader.c src/elf_loader.c)

# Set header files
include_directories(src/include)

# Add executable
add_executable(in-memory-executable-loader ${SOURCES})