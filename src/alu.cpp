#include "alu.h"

int ALU::add(int a, int b)
{
    return a + b;
}

unsigned int ALU::addressOffset(unsigned int address, int offset)
{
    int converted = 0;
    if (address <= std::numeric_limits<int>::max()) {
        converted = static_cast<int>(address);
    }
    else {
        throw std::out_of_range("Address overflow.");
    }
    converted += offset;
    if (converted < 0) {
        throw std::out_of_range("Offset address underflow.");
    }
    if (converted > std::numeric_limits<unsigned int>::max()) {
        throw std::out_of_range("Offset address overflow.");
    }
    return static_cast<unsigned int>(converted);
}

int ALU::subtract(int a, int b)
{
    return a - b;
}
