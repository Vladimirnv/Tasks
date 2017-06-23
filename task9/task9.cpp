#include "iostream"
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "..\shared\MyFile.h"
#include "..\shared\Hash.h"

#define byte unsigned char

int GetFreq(std::vector<byte> d, int *f, int n, int b, int m)
{
    int s = 0;
    if (b == 8)
    {
        m = 1;
        for (int i = 0; i < n; i += m)
        {
            uint8_t bytic = d.at(i);
            f[bytic]++;
            s++;
        }
    }
    else
    {
        if ((n % m) || m == 1) n--;
        for (int i = 0; i < n; i += m)
        {
            uint16_t bytic = (d.at(i) << 8) + d.at(i + 1);
            f[bytic]++;
            s++;
        }
    }
    return s;
}

double GetEntropy(std::vector<byte> data, int bsize, int mode) 
{
    int b = pow(2, bsize);
    int *freq = new int[b];
    int summ = 0;
    double p = 0;
    double E = 0;
    for (int i = 0; i < b; i++)
    {
        freq[i] = 0;
    }

    summ = GetFreq(data, freq, data.size(), bsize, mode);

    for (int i = 0; i < b; i++)
    {
        if (freq[i] > 0)
        {
            p = freq[i] / (double)summ;
            E += p * log(p) / log(bsize);
        }
    }

    delete [] freq;
    return -E;
}

void MakeHash()
{
    SHAHash shahash;
    MyHash myhash;
    MyFile file;
    file.Open(std::string("plain.txt"));
    std::vector<byte> data = file.GetData();
    int step = data.size() / 32;
    std::vector<byte> tmp1;
    std::vector<byte> tmp2;
    std::vector<byte> result1;
    std::vector<byte> result2;
    tmp1.resize(32);
    tmp2.resize(32);
    srand(time(NULL));
    for (int i = 0; i < step; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            tmp1.at(j) = data.at(i * 32 + j);
        }
        tmp1.push_back(rand() % 256);
        tmp1.push_back(rand() % 256);
        shahash.GenHash(tmp1);
        myhash.GenHash(tmp1, 32);
        tmp1 = shahash.GetData();
        tmp2 = myhash.GetData();
        for (int j = 0; j < 32; j++)
        {
            result1.push_back(tmp1.at(j));
            result2.push_back(tmp2.at(j));
        }
    }
    file.SetData(result1);
    file.Write(std::string("SHAhash"));
    file.SetData(result2);
    file.Write(std::string("MYhash"));
}

void MakeZip()
{
    
    /*HZIP hz = CreateZip("simple1.zip", 0);
    ZipAdd(hz, "znsimple.bmp", "simple.bmp");
    ZipAdd(hz, "znsimple.txt", "simple.txt");
    CloseZip(hz);*/
}

int main()
{
    MakeHash();
    MyFile hash;
    hash.Open(std::string("plain.txt"));
    /*std::vector<byte> data;
    std::vector<byte> d;
    int n = hash.GetData().size();
    data.resize(n * 9 / 8 + 16);
    d.resize(n);
    int *k = new int(n);
    byte *str = (byte *)malloc(1000000 * sizeof(byte));
    MakeZip();
    hash.SetData(data);
    hash.Write(std::string("result"));
    delete k;
    delete str;*/
    printf("Plaintext:\n");
    printf("1 byte: %.4f\n", GetEntropy(hash.GetData(), 8, 1));
    printf("2 byte with inter: %.4f\n", GetEntropy(hash.GetData(), 16, 1));
    printf("2 byte: %.4f\n", GetEntropy(hash.GetData(), 16, 2));
    hash.Open(std::string("SHAhash.txt"));
    printf("SHAhash:\n");
    printf("1 byte: %.4f\n", GetEntropy(hash.GetData(), 8, 1));
    printf("2 byte with inter: %.4f\n", GetEntropy(hash.GetData(), 16, 1));
    printf("2 byte: %.4f\n", GetEntropy(hash.GetData(), 16, 2));
    hash.Open(std::string("Myhash.txt"));
    printf("Myhash:\n");
    printf("1 byte: %.4f\n", GetEntropy(hash.GetData(), 8, 1));
    printf("2 byte with inter: %.4f\n", GetEntropy(hash.GetData(), 16, 1));
    printf("2 byte: %.4f\n", GetEntropy(hash.GetData(), 16, 2));
    system("pause");
    return 0;
}

