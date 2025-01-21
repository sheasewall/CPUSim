#ifndef ALU_H
#define ALU_H

#include <limits>
#include <stdexcept>

class ALU {
public:
    ALU() {}
    ~ALU() {}

    int add(int a, int b);
    unsigned int addressOffset(unsigned int address, int offset);
    int subtract(int a, int b);
};

#endif // ALU_H