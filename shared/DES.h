#include "..\third-party\src\cryptopp\des.h"

class Crypto_Des
{
public:
    Crypto_Des()
    {

    }

    bool Des_Crypto(std::vector<byte> data, std::vector<byte> key, CryptoPP::CipherDir direction)
    {
        _data.clear();
        _data = data;

        CryptoPP::BlockTransformation *cryptoalg = NULL;

        if (direction == CryptoPP::ENCRYPTION)
        {
            cryptoalg = new CryptoPP::DESEncryption(key.data(), CryptoPP::DES::KEYLENGTH);
        }
        else
        {
            cryptoalg = new CryptoPP::DESDecryption(key.data(), CryptoPP::DES::KEYLENGTH);
        }



        int countbytes = _data.size() / cryptoalg->BlockSize();

        for (int i = 0; i < countbytes + 1; i++)
        {
            int offset = i * cryptoalg->BlockSize();
            cryptoalg->ProcessBlock(_data.data() + offset);
        }
        delete cryptoalg;

        return true;
    }

    std::vector<byte> GetData() { return _data; }

private:
    std::vector<byte> _data;
};