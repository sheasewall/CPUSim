#include "controlunit.h"

int main(int, char**)
{
    ControlUnit cu("../../../data/instructions.txt", "../../../data/data.txt");
    cu.step();
    cu.print();
    cu.step();
    cu.print();
    cu.step();
    cu.print();
    cu.step();
    cu.print();
    cu.step();
    cu.print();
    cu.step();
    cu.print();
}

