#include <vector>
#define byte unsigned char

class Crypto_RC4
{
public:
    Crypto_RC4()
    {

    }

    bool RC4_Crypto(std::vector<byte> data, std::vector<byte> key, int n)
    {
        int count = pow(2, n);
        int *stable = new int[count];
        for (int i = 0; i < count; i++)
        {
            stable[i] = i;
        }
        int j = 0;
        int tmp = 0;
        for (int i = 0; i < count; i++)
        {
            j = (j + stable[i] + key.at(i % key.size())) % count;
            tmp = stable[i];
            stable[i] = stable[j];
            stable[j] = stable[i];
        }

        _data.resize(data.size());

        for (int i = 0; i < data.size(); i++)
        {
            _data.at(i) = data.at(i) ^ GetKey(stable, count);
        }

        delete[] stable;
        return true;
    }

    std::vector<byte> &GetData() { return _data; }

private:
    std::vector<byte> _data;

    byte GetKey(int *s, int n)
    {
        int i = 0;
        int j = 0;
        i = (i + 1) % n;
        j = (j + s[i]) % n;
        int tmp = s[i];
        s[i] = s[j];
        s[j] = s[i];
        return s[(s[i] + s[j]) % n];
    }
};