#include "VM.hpp"
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

VM::VM()
{
    memset(registers, 0, sizeof registers);
}

int
VM::run(uint16_t *data, size_t size)
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
VM::load(uint16_t *data, size_t size)
{
    if (size % sizeof(uint16_t))
    {
        cerr << "Error: Unaligned data. Size must be a multiple of " << sizeof(uint16_t) << endl;
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
        const uint16_t opcode = fetch();
        switch (opcode)
        {
            case  0: // halt
            return OK;

            case  6: op_jmp();
            break;
            case  7: op_jt();
            break;
            case  8: op_jf();
            break;
            case 19: op_out();
            break;
            case 21: // noop
            break;

            default: cerr << endl << "Unknown opcode: " << opcode << endl;
            return opcode;
        }
    }
}

uint16_t
VM::fetch(bool translate_register)
{
    uint16_t number = memory[program_counter++];

    // Check validity
    if (number >= INVALID_VALUE )
    {
        throw number;
    }

    // Register access
    if (translate_register)
    {
        if (number >= REGISTER_START)
        {
            return registers[number - REGISTER_START];
        }
    }

    return number;
}

uint16_t
VM::fetch(void)
{
    return fetch(true);
}

uint16_t
VM::fetch_regaddress(void)
{
    uint16_t address = fetch(false);

    // Assert address is a register
    if(address < REGISTER_START)
    {
        throw address;
    }

    return address;
}

void
VM::op_out()
{
    cout << (char) fetch();
}

void
VM::op_jmp() 
{
    program_counter = fetch();
    cerr << "Jump: " << program_counter << endl;
}

void
VM::op_jt(void)
{
    uint16_t condition = fetch();
    cerr << "If (" << condition << " != 0) ";
    if(condition != 0)
    {
        op_jmp();
    }
    else
    {
        cerr << endl;
        (void) fetch();
    }
}

void
VM::op_jf(void)
{
    uint16_t condition = fetch();
    cerr << "If (" << condition << " == 0) ";
    if(condition == 0)
    {
        op_jmp();
    }
    else
    {
        cerr << endl;
        (void) fetch();
    }
}
