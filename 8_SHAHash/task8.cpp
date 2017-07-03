#include "iostream"
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "..\third-party\src\cryptopp\sha.h"
#include "..\third-party\src\cryptopp\hex.h"
#include "..\shared\MyFile.h"

#define byte unsigned char

class SHAHash
{
public:

    bool GenHash(std::vector<byte> data)
    {
        CryptoPP::SHA256 hash;
        std::vector<byte> result;
        result.resize(CryptoPP::SHA256::DIGESTSIZE);
        hash.CalculateDigest(result.data(), data.data(), data.size());
        _data = result;
        return !_data.empty();
    }

    std::vector<byte> &GetData() { return _data; }

private:
    std::vector<byte> _data;
};

int main()
{
    SHAHash hash;
    MyFile file;
    std::string fn = "";
    int len = 32;
    printf("Input file: ");
    std::cin >> fn;
    file.Open(fn);
    std::vector<byte> data = file.GetData();
    if (hash.GenHash(data))
    {
        file.SetData(hash.GetData());
        file.Write(std::string("result"));
    }
    printf("Done!\n");
    system("pause");
    return 0;
}