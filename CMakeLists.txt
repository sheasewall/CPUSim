cmake_minimum_required(VERSION 3.31)
project(rv32sim VERSION 0.5.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Create a library for shared code
add_library(cpu_lib
    src/alu.cpp
    src/controlunit.cpp
    src/riscinstructions.cpp
)

# Link the main executable with the library
add_executable(rv32sim
    src/main.cpp
    include/file.hpp
)
target_link_libraries(rv32sim cpu_lib)

add_compile_definitions(MEMORY_FILES_DIR="${PROJECT_SOURCE_DIR}/tests/memory")
add_compile_definitions(DATA_FILES_DIR="${PROJECT_SOURCE_DIR}/data")

install(TARGETS rv32sim
    DESTINATION /home/riscofuser/.local/bin
)