cmake_minimum_required(VERSION 3.10)
project(rv32sim VERSION 0.5.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
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

set_directory_properties(PROPERTIES
  CMAKE_SUPPRESS_DEVELOPER_WARNINGS TRUE
)

# Setup Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Link test executables with the library
add_executable(alu_tests
    tests/alu_tests.cpp
)
target_link_libraries(alu_tests cpu_lib GTest::gtest_main)

add_executable(instruction_parsing_tests
    tests/instruction_parsing_tests.cpp
)
target_link_libraries(instruction_parsing_tests cpu_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(alu_tests)
gtest_discover_tests(instruction_parsing_tests)

add_compile_definitions(MEMORY_FILES_DIR="${PROJECT_SOURCE_DIR}/tests/memory")
add_compile_definitions(DATA_FILES_DIR="${PROJECT_SOURCE_DIR}/data")

install(TARGETS rv32sim
    DESTINATION /home/riscofuser/.local/bin
)