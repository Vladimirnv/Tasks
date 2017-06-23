#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

#define bytes unsigned char

class MyFile
{
public:
    MyFile()
        : _file(NULL)
    {

    }

    std::string GetEx(std::string text);
    bool Open(std::string &name);
    bool Write(std::string &name);

    void Close() { fclose(_file); };

    std::vector<bytes> &GetData() { return _data; }

    void SetData(std::vector<bytes> &data) { _data = data; }

    int &GetFileSize() { return _filesize; }

private:
    std::vector<bytes> _data;
    FILE* _file;
    std::string _ex;
    std::string path = "../res/";
    int _filesize;
};