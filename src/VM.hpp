#include <fstream>

enum ExitStatus { OK, ERROR };

class VM
{
public:
    VM(char *path) : path(path) {};
    int run(void);

private:
    std::ifstream binary;
    char *path;

    uint16_t read_number(void);
    void op_out(uint16_t character);
};
