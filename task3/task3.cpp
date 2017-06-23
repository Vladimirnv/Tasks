#include "iostream"
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "../third-party/src/cryptopp/des.h"
#include "..\shared\MyFile.h"
#include "..\shared\KeysGenerator.h"

#define _MENU "Menu: "
#define _MENU_ONE "1: Chose input file"
#define _MENU_TWO "2: Generate key"
#define _MENU_THREE "3: Encript file"
#define _MENU_FOUR "4: Decript file"
#define _MENU_FIVE "5: Exit"
#define _INPUT_DIALOG "Input path: "
#define _OUTPUT_DIALOG "Output path: "
#define _ERROR_OPEN "Can not open file"
#define _ERROR_ENCRIPT "Can not encript file"
#define _OUTPUT_OK "Done!"

class Crypto_Des
{
public:
    Crypto_Des() 
    {

    }

    bool Des_Crypto(std::vector<byte> data, std::vector<byte> key, CryptoPP::CipherDir direction)
    {
        _data = data;

        CryptoPP::BlockTransformation *cryptoalg = NULL;

        if (direction == CryptoPP::DECRYPTION)
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

    std::vector<byte> &GetData() { return _data; }

private:
    std::vector<byte> _data;
};

int main()
{
    MyFile plaintext;
    MyFile keytext;
    KeysGenerator genkey;
    Crypto_Des des;
    bool loop = true;
    std::string command = "";
    std::string path = "";
    CryptoPP::CipherDir mode;
    bool cryptoflag = false;
    while (loop)
    {
        cryptoflag = false;
        std::cout << _MENU << std::endl << _MENU_ONE << std::endl << _MENU_TWO << std::endl << 
            _MENU_THREE << std::endl << _MENU_FOUR << std::endl << _MENU_FIVE << std::endl;
        std::cout << ">> ";
        std::cin >> command;
        switch (command[0])
        {
        case '1':
            std::cout << "Plainfile " << _INPUT_DIALOG;
            std::cin >> path;
            if (!plaintext.Open(path))
            {
                std::cout << _ERROR_OPEN << std::endl;
            }
            else
            {
                std::cout << _OUTPUT_OK << std::endl;
            }
            break;
        case '2':
            if (!genkey.Generate(plaintext.GetFileSize()))
            {
                std::cout << _ERROR_OPEN << std::endl;
            }
            else
            {
                std::cout << _OUTPUT_OK << std::endl;
            }
            break;
        case '3':
            mode = CryptoPP::ENCRYPTION;
            cryptoflag = true;
            break;
        case '4':
            mode = CryptoPP::DECRYPTION;
            cryptoflag = true;
            break;

        default:
            break;
        }

        if (cryptoflag)
        {
            std::cout << "Keyfile " << _INPUT_DIALOG;
            std::cin >> path;
            if (!keytext.Open(path))
            {
                std::cout << _ERROR_OPEN << std::endl;
            }
            else
            {
                if (!des.Des_Crypto(plaintext.GetData(), keytext.GetData(), mode))
                {
                    std::cout << _ERROR_ENCRIPT << std::endl;
                }
                else
                {
                    plaintext.SetData(des.GetData());
                    plaintext.Write(std::string("result"));
                    std::cout << _OUTPUT_OK << std::endl;
                }
            }
        }

        loop = (command[0] != '5');
        system("pause");
        system("cls");
    }

    return 0;
}