#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class RiscTrapException : public std::runtime_error {
public:
    explicit RiscTrapException(const std::string& msg)
        : std::runtime_error("RISC-V Trap: " + msg) {}
};

class EcallTrap : public RiscTrapException {
public:
    EcallTrap() : RiscTrapException("ecall executed") { std::exit(0); }
};

class EbreakTrap : public RiscTrapException {
public:
    EbreakTrap() : RiscTrapException("ebreak executed") {}
};


#endif // EXCEPTIONS_H
