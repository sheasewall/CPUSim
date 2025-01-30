#include "controlunit.h"
#include "fileiounit.hpp"

ControlUnit::ControlUnit(std::string _memory_file)
    : memory_file(_memory_file), alu(), if_id(), id_ex(), ex_mem(), mem_wb()
{
    // Initialize registers
    for (int i = 0; i < 32; i++)
    {
        regs.insert({ std::bitset<5>(i), std::bitset<32>(0) });
    }

    // Initialize program counter to zero
    pc = std::bitset<32>(0);

    // Initialize memory
    readMemory();
}

void ControlUnit::step()
{
    fetch();
    decode();
    execute();
    memoryAccess();
    writeBack();
}

void ControlUnit::fetch()
{
    if_id.instruction 
    if_id.pc = pc;

}
