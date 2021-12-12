#include <fstream>

#define MEMORY_T uint16_t
#define MEM_ELEMENTS 0x8000

enum ExitStatus { OK, ERROR };

class VM
{
public:
    int run(MEMORY_T *data, size_t size);

private:
    static const MEMORY_T REGISTER_START = 32768;
    static const MEMORY_T INVALID_VALUE = 32776;
    static const MEMORY_T REG_ELEMENTS = INVALID_VALUE - REGISTER_START;

    std::ifstream binary;
    char *path;
    MEMORY_T memory[MEM_ELEMENTS];
    MEMORY_T registers[REG_ELEMENTS];
    uint16_t program_counter = 0;

    bool load(MEMORY_T *data, size_t size);
    int execute(void);
    MEMORY_T fetch(void);
    void op_out(void);
    void op_jmp(void);
    void op_jt(void);
    void op_jf(void);
};
