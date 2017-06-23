#include "..\shared\MyFile.h"

#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS

std::string MyFile::GetEx(std::string text)
{
    int i = text.length();
    std::string ext = "";
    while (i > 0)
    {
        ext = text[i] + ext;
        if (text[i] == '.')
            i = 0;
        i--;
    }
    return ext;
}

bool MyFile::Open(std::string &name)
{
    _file = fopen((path + name).c_str(), "rb");

    if (_file != NULL)
    {
        _ex = GetEx(name);
        fseek(_file, 0L, SEEK_END);
        _filesize = ftell(_file);
        _data.resize(_filesize);
        fseek(_file, 0L, SEEK_SET);
        fread(_data.data(), 1, _filesize, _file);
        Close();
    }

    return !_data.empty();
}

bool MyFile::Write(std::string &name)
{
    _file = fopen((path + name + _ex).c_str(), "wb");

    if (_file != NULL)
    {
        fwrite(_data.data(), 1, _data.size(), _file);
        Close();
    }
    return !_data.empty();
}