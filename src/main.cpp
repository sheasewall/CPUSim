#include "controlunit.h"
#include "translator.h"

int main(int, char**)
{
    Translator t;
    t.translateFile(std::string(MEMORY_FILES_DIR) + "/program");

    std::string instruction_file = std::string(MEMORY_FILES_DIR) + "/program.bin";
    std::string register_file = std::string(DATA_FILES_DIR) + "/registers.txt";
    std::string data_file = std::string(DATA_FILES_DIR) + "/data.txt";
    ControlUnit cu(instruction_file, register_file, data_file);

    std::string input;
    while (true)
    {
        // This CLI, the print(), and the dump() functions are 
        // not meant to model actual IO nor model a debugger. 
        // Instead they are godlike powers to inspect the state 
        // of the system for debugging the actual implementation.
        // The step function should be tied to a clock signal
        // in the future.
        std::cout << "Enter command (step, print, dump, quit): ";
        std::cout.flush();
        std::getline(std::cin, input);
    
        std::istringstream iss(input);
        std::string command;
        iss >> command; 
    
        if (command == "step" || command == "s")
        {
            try {
                cu.step();
            } catch (const RiscTrapException& e) {
                cu.print();
                std::cout << e.what() << std::endl;
            } catch (...) {
                std::cout << "Error executing instruction" << std::endl;
            }
        }
        else if (command == "print" || command == "p")
        {
            std::string args = "";
            iss >> args;
            cu.print((args.find("i") != std::string::npos), (args.find("r") != std::string::npos), (args.find("d") != std::string::npos));
        }
        else if (command == "dump" || command == "d")
        {
            cu.dump();
        }
        else if (command == "next" || command == "n")
        {
            while (true) {
                try {
                    cu.step();
                } catch (const EbreakTrap& e) {
                    cu.print(false, true, true);
                    std::cout << e.what() << std::endl;
                    break;
                } 
                catch (...) {
                    std::cout << "Error executing instruction" << std::endl;
                    break;
                }
            }
        }
        else if (command == "quit" || command == "q")
        {
            break;
        }
        else
        {
            std::cout << "Unknown command" << std::endl;
        }
    }
}

