#include "VM.hpp"
#include <fstream>

int main(int argc, char **argv)
{
    return (argc >= 2)
        ? VM(argv[1]).run()
        : ERROR;
}
