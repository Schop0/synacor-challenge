#include "VM.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "Error: missing file argument" << endl;
        return ERROR;
    }

    uint16_t program[MEM_ELEMENTS];
    int status = ERROR;

    ifstream file(argv[1], ios::binary | ios::ate);
    streamsize size = file.tellg();

    file.seekg(0, ios::beg);

    if (size > sizeof program)
    {
        cerr << "Error: file does not fit in program memory (" << sizeof program << " bytes)" << endl;
        return ERROR;
    }

    if (file.read((char *)program, size))
    {
        status = VM().run(program, size);
    }

    return status;
}
