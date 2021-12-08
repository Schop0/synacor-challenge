#include "VM.hpp"
#include <iostream>

int
VM::run(void)
{
    while (true)
    {
        const uint16_t opcode = read_number();
        switch (opcode)
        {
            case  0: // halt
            return OK;

            case 19: op_out(read_number());
            break;
            case 21: // noop
            break;

            default: std::cerr << std::endl << "Unknown opcode: " << opcode << std::endl;
            return opcode;
        }
    }
}

uint16_t
VM::read_number(void)
{
    // Open if needed
    if (!binary.is_open())
    {
        std::cerr << std::endl << "Opening binary file: " << path << std::endl;
        binary.open(path);
    }

    // Read word
    char buf[2];
    binary.read(buf, 2);

    // Assemble number
    int16_t number = 0;
    number += buf[0];
    number += buf[1] * UINT8_MAX;

    // Check validity
    if (number >= 32776 )
    {
        throw number;
    }

    return number;
}

void
VM::op_out(uint16_t character)
{
    std::cout << (char) character;
}
