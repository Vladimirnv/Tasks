#include "..\shared\KeysGenerator.h"

#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS

bool KeysGenerator::Generate(int size)
{
    _file_out = fopen("../res/key", "wb");

    if (_file_out != NULL)
    {
        _key.resize(size);
        char t;
        srand(time(NULL));
        for (int i = 0; i < size; i++)
        {
            t = rand() % 255;
            _key.at(i) = (t);
            fprintf(_file_out, "%c", t);
        }
        Close();
    }
    return !_key.empty();
}