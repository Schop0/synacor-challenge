#include <fstream>
#include <stack>

#define MEM_ELEMENTS 0x8000

enum ExitStatus { OK, ERROR };

class VM
{
public:
    VM();
    int run(uint16_t *data, size_t size);

private:
    static const uint16_t REGISTER_START = 32768;
    static const uint16_t INVALID_VALUE = 32776;
    static const uint16_t REG_ELEMENTS = INVALID_VALUE - REGISTER_START;

    std::ifstream binary;
    char *path;
    uint16_t memory[MEM_ELEMENTS];
    uint16_t registers[REG_ELEMENTS];
    std::stack<uint16_t> stack;
    uint16_t program_counter = 0;

    bool load(uint16_t *data, size_t size);
    int execute(void);
    uint16_t fetch(void);
    uint16_t fetch(bool translate_register);
    uint16_t fetch_regaddress(void);
    uint16_t fetch_address(void);
    void set_register(uint16_t reg, uint16_t value);
    void set(uint16_t address, uint16_t value);
    void push(uint16_t value);
    uint16_t pop(void);
    void op_out(void);
    void op_jmp(void);
    void op_jt(void);
    void op_jf(void);
    void op_set(void);
    void op_add(void);
    void op_eq(void);
    void op_push(void);
    void op_pop(void);
};
