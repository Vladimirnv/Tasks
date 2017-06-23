#include <iostream>
#include <vector>
#include <time.h>
#include <cstdlib>

#define bytes unsigned char

class KeysGenerator
{
public:
    KeysGenerator()
    {

    }

    bool Generate(int size);
    void Close() { fclose(_file_out); }

    std::vector<bytes> &GetData() { return _key; }

private:
    std::vector<bytes> _key;
    FILE* _file_out;
};