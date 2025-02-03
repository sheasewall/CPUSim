#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <bitset>

class Translator {
private:
    std::bitset<5> getRegister(std::string reg);
    std::bitset<12> getImmediate(std::string imm);
    std::bitset<12> getImmediateMulOf2(std::string imm);
    std::bitset<20> getImmediateLong(std::string imm);
    std::bitset<20> getImmediateLongMulOf2(std::string imm);

    std::string jalImmediateToString(std::bitset<20> imm);
    std::pair<std::string, std::string> branchImmediateToStrings(std::bitset<12> imm);

    std::bitset<32> generateRType(std::string instruction);
    std::bitset<32> generateIType(std::string instruction);
    std::bitset<32> generateSType(std::string instruction); 
    std::bitset<32> generateBType(std::string instruction);
    std::bitset<32> generateUType(std::string instruction);
    std::bitset<32> generateJType(std::string instruction);

public:
    Translator() {}
    ~Translator() {}

    void translate();

};

#endif // TRANSLATOR_H