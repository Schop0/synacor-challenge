#include "VM.hpp"
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

int
VM::run(MEMORY_T *data, size_t size)
{
    if (load(data, size))
    {
        return execute();
    }
    else
    {
        cerr << "Error: failed to load" << endl;
        return ERROR;
    }
}

bool
VM::load(MEMORY_T *data, size_t size)
{
    if (size % sizeof(MEMORY_T))
    {
        cerr << "Error: Unaligned data. Size must be a multiple of " << sizeof(MEMORY_T) << endl;
        return false;
    }

    if (size > sizeof memory)
    {
        cerr << "Error: size > sizeof memory (" << size << " > " << sizeof memory << ")" << endl;
        return false;
    }

    memcpy(memory, data, size);
    return true;
}

int
VM::execute(void)
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

            default: cerr << endl << "Unknown opcode: " << opcode << endl;
            return opcode;
        }
    }
}

uint16_t
VM::read_number(void)
{
    int16_t number = memory[program_counter++];

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
    cout << (char) character;
}
