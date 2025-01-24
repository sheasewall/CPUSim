#include "controlunit.h"

using ValReg = Register<int>;
using AddReg = Register<unsigned int>;

const std::string MEM_FILE = "../../../memory.txt";

ControlUnit::ControlUnit(std::string _memory_file) : A(), B(), C(), PC(), sIR(), pIR(), alu(), memory_file(_memory_file)
{
    PC.setVal(1);

    val_reg_registry["A"] = &A;
    val_reg_registry["B"] = &B;
    val_reg_registry["C"] = &C;

    readMemory();
}

ControlUnit::ControlUnit() : ControlUnit(MEM_FILE)
{

}

void ControlUnit::readMemory()
{
    std::ifstream file(memory_file);
    if (!file.is_open()) {
        std::cerr << "Unable to open " << memory_file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        unsigned int lineNumber;
        iss >> lineNumber;
        std::string instruction;
        std::getline(iss, instruction);
        if (instruction != "") {
            memory[lineNumber] = instruction;
        }
    }

    file.close();
}

int ControlUnit::peekLineMemory(unsigned int line_num)
{
    std::string line = memory.at(line_num);
    std::istringstream iss(line);
    int val;
    iss >> val;
    return val;
}

void ControlUnit::printValRegisters()
{
    std::cout << "A: " << A.getVal() << std::endl;
    std::cout << "B: " << B.getVal() << std::endl;
    std::cout << "C: " << C.getVal() << std::endl;
}

void ControlUnit::run(unsigned int line_num)
{
    PC.setVal(line_num);
    while (!isHalted())
    {
        executeInstruction();
    }
    printValRegisters();
}

bool ControlUnit::isHalted()
{
    return PC.getVal() == 0;
}

void ControlUnit::executeInstruction()
{
    fetch();
    decode();
    execute();
}

int ControlUnit::peekValRegister(std::string reg)
{
    return val_reg_registry.at(reg)->getVal();
}

void ControlUnit::fetch()
{
    readMemory();
    sIR.setVal(memory[PC.getVal()]);
}

void ControlUnit::decode()
{
    // use a string stream to read in the first word
    std::istringstream iss(sIR.getVal());
    std::string opcode;
    iss >> opcode;

    if (opcode == "NOOP")
    {
        pIR.setVal(std::make_shared<DoNothing>(DoNothing()));
    }
    else if (opcode == "MOVE")
    {
        pIR.setVal(std::make_shared<Move>(Move(iss, val_reg_registry)));
    }
    else if (opcode == "ADD")
    {
        pIR.setVal(std::make_shared<Add>(Add(iss, val_reg_registry)));
    }
    else if (opcode == "COMP")
    {
        pIR.setVal(std::make_shared<LessThanComp>(LessThanComp(iss, val_reg_registry, &compare)));
    }
    else if (opcode == "JUMPIF")
    {
        pIR.setVal(std::make_shared<JumpIf>(JumpIf(iss, val_reg_registry, &PC, &compare)));
    }
    else if (opcode == "WRITE")
    {
        pIR.setVal(std::make_shared<Write>(Write(iss, val_reg_registry, memory_file)));
    }
    else if (opcode == "READ")
    {
        pIR.setVal(std::make_shared<Read>(Read(iss, val_reg_registry, memory_file)));
    }
    // else if (opcode == "WRITEFROM")
    // {
    //     pIR.setVal(std::make_shared<WriteFrom>(WriteFrom(iss, MEM_FILE, val_reg_registry)));
    // }
    else
    {
        throw std::runtime_error("Invalid opcode: " + opcode);
    }
}

void ControlUnit::execute()
{
    // This ordering is impactful and should be assessed
    PC.setVal(PC.getVal() + 1);
    pIR.getVal()->execute();
}
