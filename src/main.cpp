#include "controlunit.h"

int main(int, char**)
{
    ControlUnit cu("../../../data/instructions.txt", "../../../data/registers.txt", "../../../data/data.txt");

    for (int i = 0; i < 14; i++) 
    {
        cu.step();
    }

    cu.dump();
}

