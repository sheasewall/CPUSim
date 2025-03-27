#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <bitset>
#include <map>
#include <fstream>
#include <sstream>

// This could be achieved in a more OO way 
// using polymorphism similar to how Instructions
// are represented in the simulator code
// Perhaps something like below
/*
class AssemblyInstruction
{
    std::string instruction;

    Tokenizer tokenizer;

public:
    AssemblyInstruction(std::string instruction) : instruction(instruction) {}
    virtual ~AssemblyInstruction() {}

    virtual std::bitset<32> encode() = 0;
};
*/

enum InstructionType
{
    R,
    I,
    S,
    B,
    U,
    J,
    IShift, // I types with shamt (effectively a 5-bit immediate)
    ILoad, // I types with parentheses
    Fence, // Ordering instructions
    System,  // Control instructions
    PseudoRR, // Pseudo instructions with two registers
};

struct FunctionCodes {
    std::bitset<7> funct7;
    std::bitset<3> funct3;
    std::bitset<7> opcode;
    InstructionType type;

    std::function<std::bitset<32>(std::vector<std::string>)> encode;
};

const std::map<std::string, FunctionCodes> instructionCodeMap = {
    {"add", {0b0000000, 0b000, 0b0110011, R}},
    {"sub", {0b0100000, 0b000, 0b0110011, R}},
    {"xor", {0b0000000, 0b100, 0b0110011, R}},
    {"or", {0b0000000, 0b110, 0b0110011, R}},
    {"and", {0b0000000, 0b111, 0b0110011, R}},
    {"sll", {0b0000000, 0b001, 0b0110011, R}},
    {"srl", {0b0000000, 0b101, 0b0110011, R}},
    {"sra", {0b0100000, 0b101, 0b0110011, R}},
    {"slt", {0b0000000, 0b010, 0b0110011, R}},
    {"sltu", {0b0000000, 0b011, 0b0110011, R}},
    {"addi", {0b0000000, 0b000, 0b0010011, I}},
    {"xori", {0b0000000, 0b100, 0b0010011, I}},
    {"ori", {0b0000000, 0b110, 0b0010011, I}},
    {"andi", {0b0000000, 0b111, 0b0010011, I}},
    {"slti", {0b0000000, 0b010, 0b0010011, I}},
    {"sltiu", {0b0000000, 0b011, 0b0010011, I}},
    {"sb", {0b0000000, 0b000, 0b0100011, S}},
    {"sh", {0b0000000, 0b001, 0b0100011, S}},
    {"sw", {0b0000000, 0b010, 0b0100011, S}},
    {"beq", {0b0000000, 0b000, 0b1100011, B}},
    {"bne", {0b0000000, 0b001, 0b1100011, B}}, 
    {"blt", {0b0000000, 0b100, 0b1100011, B}},
    {"bge", {0b0000000, 0b101, 0b1100011, B}},
    {"bltu", {0b0000000, 0b110, 0b1100011, B}},
    {"bgeu", {0b0000000, 0b111, 0b1100011, B}},
    {"lui", {0b0000000, 0b000, 0b0110111, U}},
    {"auipc", {0b0000000, 0b000, 0b0010111, U}},
    {"jal", {0b0000000, 0b000, 0b1101111, J}},
    {"slli", {0b0000000, 0b001, 0b0010011, IShift}},
    {"srli", {0b0000000, 0b101, 0b0010011, IShift}},
    {"srai", {0b0100000, 0b101, 0b0010011, IShift}},
    {"lb", {0b0000000, 0b000, 0b0000011, ILoad}},
    {"lh", {0b0000000, 0b001, 0b0000011, ILoad}},
    {"lw", {0b0000000, 0b010, 0b0000011, ILoad}},
    {"lbu", {0b0000000, 0b100, 0b0000011, ILoad}},
    {"lhu", {0b0000000, 0b101, 0b0000011, ILoad}},
    {"jalr", {0b0000000, 0b000, 0b1100111, ILoad}},
    {"ecall;", {0b0000000, 0b000, 0b1110011, System}},
    {"ebreak;", {0b0000000, 0b001, 0b1110011, System}},
    {"fence", {0b0000000, 0b000, 0b0001111, Fence}},
    {";", {0b0000000, 0b000, 0b0010011, I}},
    {"nop", {0b0000000, 0b000, 0b0010011, I}},
    {"mv", {0b0000000, 0b000, 0b0010011, I}},
    {"not", {0b0000000, 0b000, 0b0010011, I}},
    {"neg", {0b0000000, 0b000, 0b0010011, I}},
    {"negw", {0b0000000, 0b000, 0b0010011, I}},
    {"seqz", {0b0000000, 0b000, 0b0010011, I}},
    {"snez", {0b0000000, 0b000, 0b0010011, I}},
    {"sltz", {0b0000000, 0b000, 0b0010011, I}},
    {"sgtz", {0b0000000, 0b000, 0b0010011, I}},
};

class Translator {
private:
    std::vector<std::string> tokenize(std::string instruction, std::vector<char> delimiters);

    std::bitset<5> getRegister(std::string reg);
    std::bitset<12> getImmediate(std::string imm);
    std::bitset<12> getImmediateMulOf2(std::string imm);
    std::bitset<20> getImmediateLong(std::string imm);
    std::bitset<20> getImmediateLongMulOf2(std::string imm);

    // These functions use std::string operations for bit manipulation
    // This is for pure convenience, but to better model low-level 
    // operations, it should be replaced with "real" bit manipulation
    std::bitset<32> constructRType(std::bitset<7> funct7, std::bitset<5> rs2, std::bitset<5> rs1, std::bitset<3> funct3, std::bitset<5> rd, std::bitset<7> opcode);
    std::bitset<32> constructIType(std::bitset<12> imm, std::bitset<5> rs1, std::bitset<3> funct3, std::bitset<5> rd, std::bitset<7> opcode);
    std::bitset<32> constructSType(std::bitset<12> imm, std::bitset<5> rs2, std::bitset<5> rs1, std::bitset<3> funct3, std::bitset<7> opcode);
    std::bitset<32> constructBType(std::bitset<12> imm, std::bitset<5> rs2, std::bitset<5> rs1, std::bitset<3> funct3, std::bitset<7> opcode);
    std::bitset<32> constructUType(std::bitset<20> imm, std::bitset<5> rd, std::bitset<7> opcode);
    std::bitset<32> constructJType(std::bitset<20> imm, std::bitset<5> rd, std::bitset<7> opcode);
    std::bitset<32> constructIShiftType(std::bitset<7> funct7, std::bitset<12> imm, std::bitset<5> rs1, std::bitset<3> funct3, std::bitset<5> rd, std::bitset<7> opcode);

    std::bitset<32> generateRType(std::string instruction);
    std::bitset<32> generateIType(std::string instruction);
    std::bitset<32> generateSType(std::string instruction); 
    std::bitset<32> generateBType(std::string instruction);
    std::bitset<32> generateUType(std::string instruction);
    std::bitset<32> generateJType(std::string instruction);
    std::bitset<32> generateIShiftType(std::string instruction);
    std::bitset<32> generateILoadType(std::string instruction);
    std::bitset<32> generateFenceType(std::string instruction);
    std::bitset<32> generateSystemType(std::string instruction);

public:
    Translator() {}
    ~Translator() {}

    std::bitset<32> translate(std::string instruction);
    void translateFile(std::string filename);
};

#endif // TRANSLATOR_H