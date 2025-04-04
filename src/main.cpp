#include "controlunit.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <bin_file>" << std::endl;
        return 1;
    }

    std::string bin_file = argv[1];

    ControlUnit cu(bin_file);
    while (true) {
        try {
            cu.step();
        }
        catch (const EcallTrap& e) {
            // Exit on ecall
            cu.dump();
            return 0;
        }
        catch (const EbreakTrap& e) {
            // Save signature for debugging and continue on ebreak
            cu.signature();
        }
        catch (const std::exception& e) {
            // Save signature and dump state and exit on other exceptions
            std::cerr << "Error: " << e.what() << std::endl;
            cu.signature();
            cu.dump();
            return 1;
        }
        catch (...) {
            std::cerr << "Unknown error occurred while executing instructions" << std::endl;
            cu.dump();
            return 1;
        }
    }

    return 0;
}

