#include "translator.h"

#include <sstream>

std::bitset<5> Translator::getRegister(std::string reg)
{
    if (reg[0] == 'x')
    {
        return std::bitset<5>(std::stoi(reg.substr(1), nullptr, 16));
    }
    else if (reg[0] == 'd')
    {
        return std::bitset<5>(std::stoi(reg.substr(1), nullptr, 10));
    }
    else if (reg[0] == 'b')
    {
        return std::bitset<5>(std::stoi(reg.substr(1), nullptr, 2));
    }
    else
    {
        throw std::runtime_error("Invalid register: " + reg);
    }
}

std::bitset<12> Translator::getImmediate(std::string imm)
{
    if (imm[0] == 'x')
    {
        return std::bitset<12>(std::stoi(imm.substr(1), nullptr, 16));
    }
    else if (imm[0] == 'd')
    {
        return std::bitset<12>(std::stoi(imm.substr(1), nullptr, 10));
    }
    else if (imm[0] == 'b')
    {
        return std::bitset<12>(std::stoi(imm.substr(1), nullptr, 2));
    }
    else
    {
        throw std::runtime_error("Invalid 12-bit immediate: " + imm);
    }
}

std::bitset<12> Translator::getImmediateMulOf2(std::string imm)
{
    std::bitset<13> imm13;
    if (imm[0] == 'x')
    {
        imm13 = std::bitset<13>(std::stoi(imm.substr(1), nullptr, 16));
    }
    else if (imm[0] == 'd')
    {
        imm13 = std::bitset<13>(std::stoi(imm.substr(1), nullptr, 10));
    }
    else if (imm[0] == 'b')
    {
        imm13 = std::bitset<13>(std::stoi(imm.substr(1), nullptr, 2));
    }
    else
    {
        throw std::runtime_error("Invalid 13-bit immediate: " + imm);
    }

    if (imm13.test(0))
    {
        throw std::runtime_error("13-bit immediate was odd: " + imm);
    }
    else
    {
        return std::bitset<12>(imm13.to_string().substr(0, 12));
    }
}

std::bitset<20> Translator::getImmediateLong(std::string imm)
{
    if (imm[0] == 'x')
    {
        return std::bitset<20>(std::stol(imm.substr(1), nullptr, 16));
    }
    else if (imm[0] == 'd')
    {
        return std::bitset<20>(std::stol(imm.substr(1), nullptr, 10));
    }
    else if (imm[0] == 'b')
    {
        return std::bitset<20>(std::stol(imm.substr(1), nullptr, 2));
    }
    else
    {
        throw std::runtime_error("Invalid 20-bit immediate long: " + imm);
    }
}

std::bitset<20> Translator::getImmediateLongMulOf2(std::string imm)
{
    std::bitset<21> imm21;
    if (imm[0] == 'x')
    {
        imm21 = std::bitset<21>(std::stoi(imm.substr(1), nullptr, 16));
    }
    else if (imm[0] == 'd')
    {
        imm21 = std::bitset<21>(std::stoi(imm.substr(1), nullptr, 10));
    }
    else if (imm[0] == 'b')
    {
        imm21 = std::bitset<21>(std::stoi(imm.substr(1), nullptr, 2));
    }
    else
    {
        throw std::runtime_error("Invalid 21-bit immediate long: " + imm);
    }

    if (imm21.test(0))
    {
        throw std::runtime_error("21-bit immediate long was odd: " + imm);
    }
    else
    {
        return std::bitset<20>(imm21.to_string().substr(0, 20));
    }
}

std::string Translator::jalImmediateToString(std::bitset<20> imm)
{
    std::string imm_str = imm.to_string();
    std::string bit19 = imm_str.substr(0, 1);
    std::string bits18to11 = imm_str.substr(1, 8);
    std::string bit10 = imm_str.substr(9, 1);
    std::string bits9to0 = imm_str.substr(10, 10);

    return bit19 + bits9to0 + bit10 + bits18to11;
}

std::pair<std::string, std::string> Translator::branchImmediateToStrings(std::bitset<12> imm)
{
    std::string imm_str = imm.to_string();
    std::string bit11 = imm_str.substr(0, 1);
    std::string bit10 = imm_str.substr(1, 1);
    std::string bits9to4 = imm_str.substr(2, 6);
    std::string bits3to0 = imm_str.substr(8, 4);

    return { bit11 + bits9to4, bit10 + bits3to0 };
}

std::bitset<32> Translator::generateRType(std::string instruction)
{
    std::bitset<7> funct7;
    std::bitset<5> rs2;
    std::bitset<5> rs1;
    std::bitset<3> funct3;
    std::bitset<5> rd;
    std::bitset<7> opcode = std::bitset<7>(0b0110011);

    std::stringstream ss(instruction);
    std::string token;

    // Get the instruction name
    ss >> token;
    if (token == "add")
    {
        funct7 = std::bitset<7>(0b0000000);
        funct3 = std::bitset<3>(0b000);
    }
    else if (token == "sub")
    {
        funct7 = std::bitset<7>(0b0100000);
        funct3 = std::bitset<3>(0b000);
    }
    else if (token == "xor")
    {
        funct7 = std::bitset<7>(0b0000000);
        funct3 = std::bitset<3>(0b100);
    }
    else if (token == "or")
    {
        funct7 = std::bitset<7>(0b0000000);
        funct3 = std::bitset<3>(0b110);
    }
    else if (token == "and")
    {
        funct7 = std::bitset<7>(0b0000000);
        funct3 = std::bitset<3>(0b111);
    }
    else if (token == "sll")
    {
        funct7 = std::bitset<7>(0b0000000);
        funct3 = std::bitset<3>(0b001);
    }
    else if (token == "srl")
    {
        funct7 = std::bitset<7>(0b0000000);
        funct3 = std::bitset<3>(0b101);
    }
    else if (token == "sra")
    {
        funct7 = std::bitset<7>(0b0100000);
        funct3 = std::bitset<3>(0b101);
    }
    else if (token == "slt")
    {
        funct7 = std::bitset<7>(0b0000000);
        funct3 = std::bitset<3>(0b010);
    }
    else if (token == "sltu")
    {
        funct7 = std::bitset<7>(0b0000000);
        funct3 = std::bitset<3>(0b011);
    }
    else
    {
        throw std::runtime_error("Unknown instruction: " + token);
    }

    // Get the destination register 
    std::getline(ss, token, ',');
    rd = getRegister(token);
    // Get the first source register 
    std::getline(ss, token, ',');
    rs1 = getRegister(token);
    // Get the second source register 
    std::getline(ss, token, ';');
    rs2 = getRegister(token);

    return std::bitset<32>(funct7.to_string() + rs2.to_string() + rs1.to_string() + funct3.to_string() + rd.to_string() + opcode.to_string());
}

std::bitset<32> Translator::generateIType(std::string instruction)
{
    std::bitset<12> imm;
    std::bitset<5> rs1;
    std::bitset<3> funct3;
    std::bitset<5> rd;
    std::bitset<7> opcode;

    std::stringstream ss(instruction);
    std::string token;

    // Get the instruction name
    ss >> token;
    if (token == "addi")
    {
        funct3 = std::bitset<3>(0b000);
        opcode = std::bitset<7>(0b0010011);
    }
    else if (token == "xori")
    {
        funct3 = std::bitset<3>(0b100);
        opcode = std::bitset<7>(0b0010011);
    }
    else if (token == "ori")
    {
        funct3 = std::bitset<3>(0b110);
        opcode = std::bitset<7>(0b0010011);
    }
    else if (token == "andi")
    {
        funct3 = std::bitset<3>(0b111);
        opcode = std::bitset<7>(0b0010011);
    }
    else if (token == "slli")
    {
        throw std::runtime_error("slli translation not yet implemented");
        opcode = std::bitset<7>(0b0010011);
    }
    else if (token == "srli")
    {
        throw std::runtime_error("srli translation not yet implemented");
        opcode = std::bitset<7>(0b0010011);
    }
    else if (token == "srai")
    {
        throw std::runtime_error("srai translation not yet implemented");
        opcode = std::bitset<7>(0b0010011);
    }
    else if (token == "slti")
    {
        funct3 = std::bitset<3>(0b010);
        opcode = std::bitset<7>(0b0010011);
    }
    else if (token == "sltiu")
    {
        funct3 = std::bitset<3>(0b011);
        opcode = std::bitset<7>(0b0010011);
    }
    else if (token == "jalr")
    {
        opcode = std::bitset<7>(0b1100111);
        throw std::runtime_error("jalr translation not yet implemented");
    }
    else if (token == "lb")
    {
        funct3 = std::bitset<3>(0b000);
        opcode = std::bitset<7>(0b0000011);
    }
    else if (token == "lh")
    {
        funct3 = std::bitset<3>(0b001);
        opcode = std::bitset<7>(0b0000011);
    }
    else if (token == "lw")
    {
        funct3 = std::bitset<3>(0b010);
        opcode = std::bitset<7>(0b0000011);
    }
    else if (token == "lbu")
    {
        funct3 = std::bitset<3>(0b100);
        opcode = std::bitset<7>(0b0000011);
    }
    else if (token == "lhu")
    {
        funct3 = std::bitset<3>(0b101);
        opcode = std::bitset<7>(0b0000011);
    }
    else if (token == "ecall")
    {
        opcode = std::bitset<7>(0b1110011);
        throw std::runtime_error("ecall translation not yet implemented");
    }
    else if (token == "ebreak")
    {
        opcode = std::bitset<7>(0b1110011);
        throw std::runtime_error("ebreak translation not yet implemented");
    }
    else
    {
        throw std::runtime_error("Unknown instruction: " + token);
    }

    // Get the destination register 
    std::getline(ss, token, ',');
    rd = getRegister(token);
    // Get the first source register 
    std::getline(ss, token, ',');
    rs1 = getRegister(token);
    // Get the immediate 
    std::getline(ss, token, ';');
    imm = getImmediate(token);

    return std::bitset<32>(imm.to_string() + rs1.to_string() + funct3.to_string() + rd.to_string() + opcode.to_string());
}

std::bitset<32> Translator::generateSType(std::string instruction)
{
    std::bitset<12> imm;

    std::bitset<5> rs2;
    std::bitset<5> rs1;
    std::bitset<3> funct3;
    std::bitset<7> opcode = std::bitset<7>(0b0100011);

    std::stringstream ss(instruction);
    std::string token;

    // Get the instruction name
    ss >> token;
    if (token == "sb")
    {
        funct3 = std::bitset<3>(0b000);
    }
    else if (token == "sh")
    {
        funct3 = std::bitset<3>(0b001);
    }
    else if (token == "sw")
    {
        funct3 = std::bitset<3>(0b010);
    }
    else
    {
        throw std::runtime_error("Unknown instruction: " + token);
    }

    // Get the data register
    std::getline(ss, token, ',');
    rs2 = getRegister(token);
    // Get the offset
    std::getline(ss, token, '(');
    imm = getImmediate(token);
    // Get the address register
    std::getline(ss, token, ')');
    rs1 = getRegister(token);

    return std::bitset<32>(imm.to_string().substr(0, 7) + rs2.to_string() + rs1.to_string() + funct3.to_string() + imm.to_string().substr(7, 5) + opcode.to_string());
}

std::bitset<32> Translator::generateBType(std::string instruction)
{
    std::bitset<12> imm;

    std::bitset<5> rs2;
    std::bitset<5> rs1;
    std::bitset<3> funct3;
    std::bitset<7> opcode = std::bitset<7>(0b1100011);

    std::stringstream ss(instruction);
    std::string token;

    // Get the instruction name
    ss >> token;
    if (token == "beq")
    {
        funct3 = std::bitset<3>(0b000);
    }
    else if (token == "bne")
    {
        funct3 = std::bitset<3>(0b001);
    }
    else if (token == "blt")
    {
        funct3 = std::bitset<3>(0b100);
    }
    else if (token == "bge")
    {
        funct3 = std::bitset<3>(0b101);
    }
    else if (token == "bltu")
    {
        funct3 = std::bitset<3>(0b110);
    }
    else if (token == "bgeu")
    {
        funct3 = std::bitset<3>(0b111);
    }
    else
    {
        throw std::runtime_error("Unknown instruction: " + token);
    }

    // Get the first source register
    std::getline(ss, token, ',');
    rs1 = getRegister(token);
    // Get the second source register
    std::getline(ss, token, ',');
    rs2 = getRegister(token);
    // Get the offset
    std::getline(ss, token, ';');
    imm = getImmediateMulOf2(token);
    auto imm_strings = branchImmediateToStrings(imm);

    return std::bitset<32>(imm_strings.first
        + rs2.to_string() + rs1.to_string() + funct3.to_string()
        + imm_strings.second + opcode.to_string());
}

std::bitset<32> Translator::generateUType(std::string instruction)
{
    std::bitset<20> imm;
    std::bitset<5> rd;
    std::bitset<7> opcode;

    std::stringstream ss(instruction);
    std::string token;

    // Get the instruction name
    ss >> token;
    if (token == "lui")
    {
        opcode = std::bitset<7>(0b0110111);
    }
    else if (token == "auipc")
    {
        opcode = std::bitset<7>(0b0010111);
    }
    else
    {
        throw std::runtime_error("Unknown instruction: " + token);
    }

    // Get the destination register 
    std::getline(ss, token, ',');
    rd = getRegister(token);
    // Get the immediate/offset
    std::getline(ss, token, ';');
    imm = getImmediateLong(token);

    return std::bitset<32>(imm.to_string() + rd.to_string() + opcode.to_string());
}

std::bitset<32> Translator::generateJType(std::string instruction)
{
    std::bitset<20> imm;
    std::bitset<5> rd;
    std::bitset<7> opcode;

    std::stringstream ss(instruction);
    std::string token;

    // Get the instruction name
    ss >> token;
    if (token == "jal")
    {
        opcode = std::bitset<7>(0b1101111);
    }
    else
    {
        throw std::runtime_error("Unknown instruction: " + token);
    }

    // Get the destination register 
    std::getline(ss, token, ',');
    rd = getRegister(token);
    // Get the immediate/offset
    std::getline(ss, token, ';');
    imm = getImmediateLongMulOf2(token);

    return std::bitset<32>(jalImmediateToString(imm) + rd.to_string() + opcode.to_string());
}