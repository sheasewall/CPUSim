#include "controlunit.h"

ControlUnit::ControlUnit(std::string bin_file) {
  cycles = 0;
  pc = std::bitset<32>(0);
  p_mu = std::make_shared<MaskingUnit>();
  p_instruction_file = std::make_shared<InstructionFile>(bin_file);
  p_igu = std::make_shared<ImmGenUnit>();
  p_reg_file = std::make_shared<RegisterFile>();
  p_alu = std::make_shared<ALU>();
  p_data_file = std::make_shared<MemoryFile>(bin_file);
}

void ControlUnit::step() {
  fetch();
  decode();
  execute();
  memoryAccess();
  writeBack();
  cycles++;
}

void ControlUnit::fetch() {
  std::bitset<32> instruction = p_instruction_file->read(pc);
  p_current_instruction = createInstruction(instruction);
  p_current_instruction->fetch(instruction, p_mu);
}

void ControlUnit::decode() { p_current_instruction->decode(p_reg_file, p_igu); }

void ControlUnit::execute() { p_current_instruction->execute(p_alu, pc); }

void ControlUnit::memoryAccess() {
  p_current_instruction->accessMemory(p_data_file);
}

void ControlUnit::writeBack() { p_current_instruction->writeBack(p_reg_file); }

std::shared_ptr<RISC::Instruction>
ControlUnit::createInstruction(std::bitset<32> instruction) {
  std::bitset<7> opcode = p_mu->hardwareMaskBits<7, 32>(instruction, 0, 7);
  std::bitset<3> funct3 = p_mu->hardwareMaskBits<3, 32>(instruction, 12, 3);
  std::bitset<7> funct7 = p_mu->hardwareMaskBits<7, 32>(instruction, 25, 7);
  std::shared_ptr<RISC::Instruction> p_instruction;

  switch (opcode.to_ulong()) {
  case 0b0110011: {
    // RType
    if (funct3 == 0b000 && funct7 == 0b0000000) {
      p_instruction = std::make_shared<RISC::Add>();
    } else if (funct3 == 0b000 && funct7 == 0b0100000) {
      p_instruction = std::make_shared<RISC::Sub>();
    } else if (funct3 == 0b001 && funct7 == 0b0000000) {
      p_instruction = std::make_shared<RISC::ShiftLeftLogi>();
    } else if (funct3 == 0b010 && funct7 == 0b0000000) {
      p_instruction = std::make_shared<RISC::SetLessThan>();
    } else if (funct3 == 0b011 && funct7 == 0b0000000) {
      p_instruction = std::make_shared<RISC::SetLessThanUnsigned>();
    } else if (funct3 == 0b100 && funct7 == 0b0000000) {
      p_instruction = std::make_shared<RISC::Xor>();
    } else if (funct3 == 0b101 && funct7 == 0b0000000) {
      p_instruction = std::make_shared<RISC::ShiftRightLogi>();
    } else if (funct3 == 0b101 && funct7 == 0b0100000) {
      p_instruction = std::make_shared<RISC::ShiftRightArith>();
    } else if (funct3 == 0b110 && funct7 == 0b0000000) {
      p_instruction = std::make_shared<RISC::Or>();
    } else if (funct3 == 0b111 && funct7 == 0b0000000) {
      p_instruction = std::make_shared<RISC::And>();
    } else {
      throw std::runtime_error("Unknown R-type instruction: " +
                               instruction.to_string());
    }
    break;
  }
  case 0b0010011: {
    // IType
    if (funct3 == 0b000) {
      p_instruction = std::make_shared<RISC::AddImm>();
    } else if (funct3 == 0b100) {
      p_instruction = std::make_shared<RISC::XorImm>();
    } else if (funct3 == 0b110) {
      p_instruction = std::make_shared<RISC::OrImm>();
    } else if (funct3 == 0b111) {
      p_instruction = std::make_shared<RISC::AndImm>();
    } else if (funct3 == 0b010) {
      p_instruction = std::make_shared<RISC::SetLessThanImm>();
    } else if (funct3 == 0b011) {
      p_instruction = std::make_shared<RISC::SetLessThanImmUnsigned>();
    } else if (funct3 == 0b001 && funct7 == 0b0000000) {
      p_instruction = std::make_shared<RISC::ShiftLeftLogiImm>();
    } else if (funct3 == 0b101 && funct7 == 0b0000000) {
      p_instruction = std::make_shared<RISC::ShiftRightLogiImm>();
    } else if (funct3 == 0b101 && funct7 == 0b0100000) {
      p_instruction = std::make_shared<RISC::ShiftRightArithImm>();
    } else {
      throw std::runtime_error("Unknown I-type instruction: " +
                               instruction.to_string());
    }
    break;
  }
  case 0b0000011: {
    // IType (Load)
    if (funct3 == 0b010) {
      p_instruction = std::make_shared<RISC::LoadWord>();
    } else if (funct3 == 0b001) {
      p_instruction = std::make_shared<RISC::LoadHalfWord>();
    } else if (funct3 == 0b000) {
      p_instruction = std::make_shared<RISC::LoadByte>();
    } else if (funct3 == 0b101) {
      p_instruction = std::make_shared<RISC::LoadUnsignedHalfWord>();
    } else if (funct3 == 0b100) {
      p_instruction = std::make_shared<RISC::LoadUnsignedByte>();
    } else {
      throw std::runtime_error("Unknown I-type load instruction: " +
                               instruction.to_string());
    }
    break;
  }
  case 0b0100011: {
    // SType
    if (funct3 == 0b000) {
      p_instruction = std::make_shared<RISC::SaveByte>();
    } else if (funct3 == 0b001) {
      p_instruction = std::make_shared<RISC::SaveHalfWord>();
    } else if (funct3 == 0b010) {
      p_instruction = std::make_shared<RISC::SaveWord>();
    } else {
      throw std::runtime_error("Unknown S-type instruction: " +
                               instruction.to_string());
    }
    break;
  }
  case 0b1100011: {
    // BType
    if (funct3 == 0b000) {
      p_instruction = std::make_shared<RISC::BranchEqual>();
    } else if (funct3 == 0b001) {
      p_instruction = std::make_shared<RISC::BranchNotEqual>();
    } else if (funct3 == 0b100) {
      p_instruction = std::make_shared<RISC::BranchLessThan>();
    } else if (funct3 == 0b101) {
      p_instruction = std::make_shared<RISC::BranchGreaterThanEqual>();
    } else if (funct3 == 0b110) {
      p_instruction = std::make_shared<RISC::BranchLessThanUnsigned>();
    } else if (funct3 == 0b111) {
      p_instruction = std::make_shared<RISC::BranchGreaterThanEqualUnsigned>();
    } else {
      throw std::runtime_error("Unknown B-type instruction: " +
                               instruction.to_string());
    }
    break;
  }
  case 0b0110111: {
    // UType (LoadUpperImmediate)
    p_instruction = std::make_shared<RISC::LoadUpperImmediate>();
    break;
  }
  case 0b0010111: {
    // UType (AUIPC)
    p_instruction = std::make_shared<RISC::AddUpperImmedateToPC>();
    break;
  }
  case 0b1101111: {
    // JType (JumpAndLink)
    p_instruction = std::make_shared<RISC::JumpAndLink>();
    break;
  }
  case 0b1100111: {
    // IType (JumpAndLinkRegister)
    p_instruction = std::make_shared<RISC::JumpAndLinkReg>();
    break;
  }
  case 0b1110011: {
    // System (IType)
    std::bitset<12> funct12 = p_mu->hardwareMaskBits<12, 32>(instruction, 20, 12);
    if (funct3 == 000) {
      if (funct12 == 0b000000000000) {
        // Ecall
        p_instruction = std::make_shared<RISC::Ecall>();
      } else if (funct12 == 0b000000000001) {
        // Ebreak
        p_instruction = std::make_shared<RISC::Ebreak>();
      }
    } else {
      throw std::runtime_error("Unknown system instruction: " +
                               instruction.to_string());
    }
    break;
  }
  case 0b0001111: {
    // IType (Fence)
    p_instruction = std::make_shared<RISC::Fence>();
    break;
  }
  default:
    throw std::runtime_error("Unknown instruction: " + instruction.to_string());
  }

  return p_instruction;
}

void ControlUnit::signature() {
  std::ofstream signature_file("DUT-rv32sim.signature");
  std::string signature = p_data_file->signature();

  std::stringstream ss(signature);
  std::string line;
  int number_of_lines = 0;
  while (std::getline(ss, line)) {
    number_of_lines++;
  }
  signature_file << p_data_file->signature();
  // insert empty lines to reach a multiple of 4 (lines)
  for (int i = number_of_lines % 4; (i > 0) && (i < 4); i++) {
    signature_file << "00000000\n";
  }
}