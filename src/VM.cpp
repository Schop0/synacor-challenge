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

            case  1: op_set();
            break;
            case  2: op_push();
            break;
            case  3: op_pop();
            break;
            case  4: op_eq();
            break;
            case  5: op_gt();
            break;
            case  6: op_jmp();
            break;
            case  7: op_jt();
            break;
            case  8: op_jf();
            break;
            case  9: op_add();
            break;
            case 10: op_mult();
            break;
            case 11: op_mod();
            break;
            case 12: op_and();
            break;
            case 13: op_or();
            break;
            case 14: op_not();
            break;
            case 15: op_rmem();
            break;
            case 16: op_wmem();
            break;
            case 17: op_call();
            break;
            case 18: op_ret();
            break;
            case 19: op_out();
            break;
            case 20: op_in();
            break;
            case 21: // noop
            break;

            default: op_unknown(opcode);
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

uint16_t VM::fetch_address(void)
{
    return fetch(false);
}

void
VM::set_register(uint16_t reg, uint16_t value)
{
    // Translate address to index
    if (reg >= REGISTER_START)
    {
        reg -= REGISTER_START;
    }

    // Bounds check
    if (reg >= REG_ELEMENTS)
    {
        throw reg;
    }

    registers[reg] = value;
}

void
VM::set(uint16_t address, uint16_t value)
{
    if (address >= REGISTER_START)
    {
        set_register(address, value);
    }
    else
    {
        memory[address] = value;
    }
}

void
VM::push(uint16_t value)
{
    stack.push(value);
}

uint16_t
VM::pop(void)
{
    const uint16_t value = stack.top();
    stack.pop();
    return value;
}

void
VM::jump(uint16_t address)
{
    program_counter = address;
    cerr << "Jump: " << program_counter << endl;
}

uint16_t
VM::modulo_15bit(uint16_t value)
{
    return value & INT16_MAX;
}

void
VM::op_out()
{
    cout << (char) fetch();
}

void
VM::op_jmp()
{
    jump(fetch_address());
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

void
VM::op_set(void)
{
    uint16_t reg = fetch_address();
    uint16_t val = fetch();

    set_register(reg, val);
}

void
VM::op_add(void)
{
    uint16_t address = fetch_address();
    uint16_t value = modulo_15bit(fetch() + fetch());
    set(address, value);
}

void
VM::op_eq(void)
{
    uint16_t address = fetch_address();
    set(address, fetch() == fetch());
}

void VM::op_gt(void)
{
    uint16_t address = fetch_address();
    set(address, fetch() > fetch());
}

void
VM::op_push(void)
{
    push(fetch());
}

void VM::op_pop(void)
{
    set(fetch_address(),pop());
}

void VM::op_mult(void)
{
    uint16_t address = fetch_address();
    uint16_t value = modulo_15bit(fetch() * fetch());
    set(address, value);
}

void VM::op_mod(void)
{
    uint16_t address = fetch_address();
    set(address, fetch() % fetch());
}

void VM::op_and(void)
{
    uint16_t address = fetch_address();
    set(address, fetch() & fetch());
}

void VM::op_or(void)
{
    uint16_t address = fetch_address();
    set(address, fetch() | fetch());
}

void VM::op_not(void)
{
    uint16_t address = fetch_address();
    uint16_t value = modulo_15bit(~fetch());
    set(address, value);
}

void VM::op_rmem(void)
{
    op_unknown(15);
}

void VM::op_wmem(void)
{
    op_unknown(16);
}

void VM::op_call(void)
{
    cerr << "Call from: " << program_counter << " ";
    uint16_t address = fetch();
    push(program_counter);
    jump(address);
}

void VM::op_ret(void)
{
    op_unknown(18);
}

void VM::op_in(void)
{
    op_unknown(20);
}

void VM::op_unknown(uint16_t opcode)
{
    cerr << endl << "Unknown opcode: " << opcode << endl;
    exit(opcode);
}
