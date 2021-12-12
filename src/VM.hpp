#include <fstream>

#define MEMORY_T uint16_t
#define MEM_ELEMENTS 0x8000

enum ExitStatus { OK, ERROR };

class VM
{
public:
    int run(MEMORY_T *data, size_t size);

private:
    std::ifstream binary;
    char *path;
    MEMORY_T memory[MEM_ELEMENTS];
    uint16_t program_counter = 0;

    bool load(MEMORY_T *data, size_t size);
    int execute(void);
    uint16_t read_number(void);
    void op_out(uint16_t character);
};
