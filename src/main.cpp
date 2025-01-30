#include <iostream>
#include "controlunit.h"

int main(int, char**)
{
    std::cout << "Hello, from CPUSim!\n";
    // Example usage of CPU and Memory classes
    // ControlUnit controlUnit;
    // while (!controlUnit.isHalted())
    // {
    //     std::string input;
    //     std::cout << "Press Enter to execute the next instruction or type 'exit' to quit: ";
    //     std::getline(std::cin, input);
    //     if (input == "exit")
    //     {
    //         break;
    //     }
    //     controlUnit.executeInstruction();
    //     controlUnit.printValRegisters();
    // }

    // while (true)
    // {
    //     std::string input;
    //     std::cout << "Enter the line number to run from: ";
    //     std::getline(std::cin, input);
    //     if (input == "exit")
    //     {
    //         break;
    //     }
    //     unsigned int line_num = std::stoi(input);
    //     controlUnit.run(line_num);
    // }

    bool quit = false;
    while (!quit)
    {
        std::string input;
        std::cout << "Enter the line number to run from: ";
        std::getline(std::cin, input);
        if (input == "exit")
        {
            quit = true;
        }
        unsigned int line_num = std::stoi(input);
        // controlUnit.run(line_num);
    }
}

