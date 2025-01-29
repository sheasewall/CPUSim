cmake_minimum_required(VERSION 3.10)
project(CPUSim VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)


add_executable(CPUSim
    src/main.cpp
    src/controlunit.cpp
    src/alu.cpp
    include/fileiounit.hpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(alu_tests
    tests/alu_tests.cpp
    src/alu.cpp
)

target_link_libraries(alu_tests GTest::gtest_main)

add_executable(controlunit_tests
    tests/controlunit_tests.cpp
    src/controlunit.cpp
)

target_link_libraries(controlunit_tests GTest::gtest_main)

add_executable(instruction_parsing_tests
    tests/instruction_parsing_tests.cpp
    include/instruction.h
)

target_link_libraries(instruction_parsing_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(alu_tests)
gtest_discover_tests(controlunit_tests)

add_compile_definitions(MEMORY_FILES_DIR="${PROJECT_SOURCE_DIR}/tests/memory")