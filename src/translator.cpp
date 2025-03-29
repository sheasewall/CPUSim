#include "translator.h"

std::vector<std::string> Translator::tokenize(std::string instruction, std::vector<char> delimiters)
{
    std::stringstream ss(instruction);
    std::vector<std::string> tokens;
    std::string token;

    for (char delimiter : delimiters)
    {
        std::getline(ss, token, delimiter);
        // There may or may not be a space between comma and operand
        if (token[0] == ' ') {
            token = token.substr(1);
        }
        tokens.push_back(token);
    }

    return tokens;
}

std::bitset<5> Translator::getRegister(std::string reg)
{
    return std::bitset<5>(std::stoi(reg.substr(1)));
}

std::bitset<12> Translator::getImmediate(std::string imm)
{
    int imm_val = std::stoi(imm);
    if (imm_val >= 2048) { // 2^11
        throw std::invalid_argument("Immediate value is too large");
    }
    return std::bitset<12>(imm_val);
}

std::bitset<12> Translator::getImmediateMulOf2(std::string imm)
{
    int imm_val = std::stoi(imm);
    if (imm_val >= 4096) { // 2^12
        throw std::invalid_argument("Immediate value is too large");
    }
    return std::bitset<12>(imm_val >> 1);
}

std::bitset<20> Translator::getImmediateLong(std::string imm)
{
    int imm_val = stol(imm);
    if (imm_val >= 524288) { // 2^19
        throw std::invalid_argument("Immediate value is too large");
    }
    return std::bitset<20>(imm_val);
}

std::bitset<20> Translator::getImmediateLongMulOf2(std::string imm)
{
    int imm_val = stol(imm);
    if (imm_val >= 1048576) { // 2^20
        throw std::invalid_argument("Immediate value is too large");
    }
    return std::bitset<20>(imm_val >> 1);
}

std::bitset<32> Translator::constructRType(std::bitset<7> funct7, std::bitset<5> rs2, std::bitset<5> rs1, std::bitset<3> funct3, std::bitset<5> rd, std::bitset<7> opcode)
{
    return std::bitset<32>(funct7.to_string() + rs2.to_string() + rs1.to_string() + funct3.to_string() + rd.to_string() + opcode.to_string());
}

std::bitset<32> Translator::constructIType(std::bitset<12> imm, std::bitset<5> rs1, std::bitset<3> funct3, std::bitset<5> rd, std::bitset<7> opcode)
{
    return std::bitset<32>(imm.to_string() + rs1.to_string() + funct3.to_string() + rd.to_string() + opcode.to_string());
}

std::bitset<32> Translator::constructSType(std::bitset<12> imm, std::bitset<5> rs2, std::bitset<5> rs1, std::bitset<3> funct3, std::bitset<7> opcode)
{
    return std::bitset<32>(imm.to_string().substr(0, 7) + rs2.to_string() + rs1.to_string() + funct3.to_string() + imm.to_string().substr(7, 5) + opcode.to_string());
}

std::bitset<32> Translator::constructBType(std::bitset<12> imm, std::bitset<5> rs2, std::bitset<5> rs1, std::bitset<3> funct3, std::bitset<7> opcode)
{
    std::string imm_str = imm.to_string();
    std::string bit11 = imm_str.substr(0, 1);
    std::string bit10 = imm_str.substr(1, 1);
    std::string bits9to4 = imm_str.substr(2, 6);
    std::string bits3to0 = imm_str.substr(8, 4);

    return std::bitset<32>(bit11 + bits9to4
        + rs2.to_string() + rs1.to_string() + funct3.to_string()
        + bits3to0 + bit10 + opcode.to_string());
}

std::bitset<32> Translator::constructUType(std::bitset<20> imm, std::bitset<5> rd, std::bitset<7> opcode)
{
    return std::bitset<32>(imm.to_string() + rd.to_string() + opcode.to_string());
}

std::bitset<32> Translator::constructJType(std::bitset<20> imm, std::bitset<5> rd, std::bitset<7> opcode)
{
    std::string imm_str = imm.to_string();
    std::string bit19 = imm_str.substr(0, 1);
    std::string bits18to11 = imm_str.substr(1, 8);
    std::string bit10 = imm_str.substr(9, 1);
    std::string bits9to0 = imm_str.substr(10, 10);

    return std::bitset<32>(bit19 + bits9to0 + bit10 + bits18to11 + rd.to_string() + opcode.to_string());
}

std::bitset<32> Translator::constructIShiftType(std::bitset<7> funct7, std::bitset<12> imm, std::bitset<5> rs1, std::bitset<3> funct3, std::bitset<5> rd, std::bitset<7> opcode)
{
    std::bitset<5> shamt = std::bitset<5>(imm.to_string().substr(7, 5));
    return std::bitset<32>(funct7.to_string() + shamt.to_string() + rs1.to_string() + funct3.to_string() + rd.to_string() + opcode.to_string());
}

std::bitset<32> Translator::generateRType(std::string instruction)
{
    auto tokens = tokenize(instruction, { ' ', ',', ',', ';' });
    if (tokens.size() < 4) {
        throw std::runtime_error("Unknown R-type instruction: " + instruction);
    }
    FunctionCodes funct_codes = instructionCodeMap.at(tokens[0]);

    std::bitset<7> funct7 = funct_codes.funct7;
    std::bitset<3> funct3 = funct_codes.funct3;
    std::bitset<7> opcode = funct_codes.opcode;

    // Get the destination register 
    std::bitset<5> rd = getRegister(tokens[1]);
    // Get the first source register 
    std::bitset<5> rs1 = getRegister(tokens[2]);
    // Get the second source register 
    std::bitset<5> rs2 = getRegister(tokens[3]);

    return constructRType(funct7, rs2, rs1, funct3, rd, opcode);
}

std::bitset<32> Translator::generateIType(std::string instruction)
{
    auto tokens = tokenize(instruction, { ' ', ',', ',', ';' });
    if (tokens.size() < 4) {
        throw std::runtime_error("Unknown I-type instruction: " + instruction);
    }
    FunctionCodes funct_codes = instructionCodeMap.at(tokens[0]);

    std::bitset<3> funct3 = funct_codes.funct3;
    std::bitset<7> opcode = funct_codes.opcode;

    // Comment delimiter
    if (tokens[0] == ";") {
        return constructIType(std::bitset<12>(0), std::bitset<5>(0), funct3, std::bitset<5>(0), opcode);
    }

    // Get the destination register 
    std::bitset<5> rd = getRegister(tokens[1]);
    // Get the first source register 
    std::bitset<5> rs1 = getRegister(tokens[2]);
    // Get the immediate 
    std::bitset<12> imm = getImmediate(tokens[3]);

    return constructIType(imm, rs1, funct3, rd, opcode);
}

std::bitset<32> Translator::generateSType(std::string instruction)
{
    auto tokens = tokenize(instruction, { ' ', ',', '(', ')' });
    if (tokens.size() < 4) {
        throw std::runtime_error("Unknown save instruction: " + instruction);
    }
    FunctionCodes funct_codes = instructionCodeMap.at(tokens[0]);

    std::bitset<3> funct3 = funct_codes.funct3;
    std::bitset<7> opcode = funct_codes.opcode;

    // Get the data register
    std::bitset<5> rs2 = getRegister(tokens[1]);
    // Get the offset
    std::bitset<12> imm = getImmediate(tokens[2]);
    // Get the address register
    std::bitset<5> rs1 = getRegister(tokens[3]);

    return constructSType(imm, rs2, rs1, funct3, opcode);
}

std::bitset<32> Translator::generateBType(std::string instruction)
{
    auto tokens = tokenize(instruction, { ' ', ',', ',', ';' });
    if (tokens.size() < 4) {
        throw std::runtime_error("Unknown branch instruction: " + instruction);
    }
    FunctionCodes funct_codes = instructionCodeMap.at(tokens[0]);

    std::bitset<3> funct3 = funct_codes.funct3;
    std::bitset<7> opcode = funct_codes.opcode;

    // Get the first source register
    std::bitset<5> rs1 = getRegister(tokens[1]);
    // Get the second source register
    std::bitset<5> rs2 = getRegister(tokens[2]);
    // Get the offset
    std::bitset<12> imm = getImmediateMulOf2(tokens[3]);

    return constructBType(imm, rs2, rs1, funct3, opcode);
}

std::bitset<32> Translator::generateUType(std::string instruction)
{
    auto tokens = tokenize(instruction, { ' ', ',', ';' });
    if (tokens.size() < 3) {
        throw std::runtime_error("Unknown long instruction: " + instruction);
    }
    FunctionCodes funct_codes = instructionCodeMap.at(tokens[0]);

    std::bitset<7> opcode = funct_codes.opcode;

    // Get the destination register 
    std::bitset<5> rd = getRegister(tokens[1]);
    // Get the immediate/offset
    std::bitset<20> imm = getImmediateLong(tokens[2]);

    return constructUType(imm, rd, opcode);
}

std::bitset<32> Translator::generateJType(std::string instruction)
{
    auto tokens = tokenize(instruction, { ' ', ',', ';' });
    if (tokens.size() < 3) {
        throw std::runtime_error("Unknown jump instruction: " + instruction);
    }
    FunctionCodes funct_codes = instructionCodeMap.at(tokens[0]);

    std::bitset<7> opcode = funct_codes.opcode;

    // Get the destination register 
    std::bitset<5> rd = getRegister(tokens[1]);
    // Get the immediate/offset
    std::bitset<20> imm = getImmediateLongMulOf2(tokens[2]);

    return constructJType(imm, rd, opcode);
}

std::bitset<32> Translator::generateIShiftType(std::string instruction)
{
    auto tokens = tokenize(instruction, { ' ', ',', ',', ';' });
    if (tokens.size() < 4) {
        throw std::runtime_error("Unknown shift instruction: " + instruction);
    }
    FunctionCodes funct_codes = instructionCodeMap.at(tokens[0]);

    std::bitset<7> funct7 = funct_codes.funct7;
    std::bitset<3> funct3 = funct_codes.funct3;
    std::bitset<7> opcode = funct_codes.opcode;

    // Get the destination register 
    std::bitset<5> rd = getRegister(tokens[1]);
    // Get the first source register 
    std::bitset<5> rs1 = getRegister(tokens[2]);
    // Get the immediate 
    std::bitset<12> imm = getImmediate(tokens[3]);

    return constructIShiftType(funct7, imm, rs1, funct3, rd, opcode);
}

std::bitset<32> Translator::generateILoadType(std::string instruction)
{
    auto tokens = tokenize(instruction, { ' ', ',', '(', ')' });
    if (tokens.size() < 4) {
        throw std::runtime_error("Unknown load instruction: " + instruction);
    }

    FunctionCodes funct_codes = instructionCodeMap.at(tokens[0]);

    std::bitset<3> funct3 = funct_codes.funct3;
    std::bitset<7> opcode = funct_codes.opcode;

    // Get the destination register 
    std::bitset<5> rd = getRegister(tokens[1]);
    // Get the immediate 
    std::bitset<12> imm = getImmediate(tokens[2]);
    // Get the first source register 
    std::bitset<5> rs1 = getRegister(tokens[3]);

    return constructIType(imm, rs1, funct3, rd, opcode);
}

// TODO: Implement fence translation
std::bitset<32> Translator::generateFenceType(std::string instruction)
{
    throw std::runtime_error("fence translation not yet implemented");
}

// TODO: Implement system translation
std::bitset<32> Translator::generateSystemType(std::string instruction)
{
    if (instruction.substr(0, 6) == "ecall;") {
        return std::bitset<32>(0b00000000000000000000000001110011);
    }
    else if (instruction.substr(0, 7) == "ebreak;") {
        return std::bitset<32>(0b00000000000100000000000001110011);
    }
    else {
        throw std::runtime_error("Unknown system instruction: " + instruction);
    }
}

std::bitset<32> Translator::translate(std::string instruction)
{
    std::bitset<32> translated;

    std::stringstream ss(instruction);
    std::string token;

    // Get the instruction name
    ss >> token;
    // This system is ugly, should be revisited
    // Perhaps we can map directly to the 
    // generator function
    switch (instructionCodeMap.at(token).type)
    {
    case R:
        translated = generateRType(instruction);
        break;
    case I:
        translated = generateIType(instruction);
        break;
    case S:
        translated = generateSType(instruction);
        break;
    case B:
        translated = generateBType(instruction);
        break;
    case U:
        translated = generateUType(instruction);
        break;
    case J:
        translated = generateJType(instruction);
        break;
    case ILoad:
        translated = generateILoadType(instruction);
        break;
    case IShift:
        translated = generateIShiftType(instruction);
        break;
    case Fence:
        translated = generateFenceType(instruction);
        break;
    case System:
        translated = generateSystemType(instruction);
        break;
    default:
        throw std::runtime_error("Unknown instruction type: " + token);
    }

    return translated;
}

void Translator::translateFile(std::string filename)
{
    std::ifstream input_file(filename + ".asm");
    if (!input_file.is_open()) {
        throw std::runtime_error("Could not open input file: " + filename + ".asm");
    }

    std::string output_file_name = filename + ".bin";
    std::ofstream output_file(output_file_name);
    if (!output_file.is_open()) {
        throw std::runtime_error("Could not open output file: " + output_file_name + ".bin");
    }

    std::string instruction;
    while (std::getline(input_file, instruction)) {
        std::bitset<32> binary_instruction = translate(instruction);

        for (int i = 3; 0 <= i; i--) {
            std::bitset<8> current_byte = std::bitset<8>(binary_instruction.to_string().substr(i * 8, 8));
            output_file.write(reinterpret_cast<const char*>(&current_byte), 1);
        }
    }
}
