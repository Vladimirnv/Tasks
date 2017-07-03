#include "iostream"
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../third-party/src/cryptopp/rsa.h"
#include "../third-party/src/cryptopp/osrng.h"
#include "../third-party/src/cryptopp/files.h"
#include "../third-party/src/cryptopp/base64.h"
#include "..\shared\MyFile.h"

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

class KeysGenerator
{
public:
    KeysGenerator()
    {

    }

    bool Generate(int size)
    {
        CryptoPP::AutoSeededRandomPool rand;
        CryptoPP::InvertibleRSAFunction privatekey;
        privatekey.Initialize(rand, size);
        CryptoPP::Base64Encoder privatekeyfile(new CryptoPP::FileSink("../res/privatekey"));
        privatekey.DEREncode(privatekeyfile);
        privatekeyfile.MessageEnd();
        CryptoPP::RSAFunction pubkey(privatekey);
        CryptoPP::Base64Encoder publickeyfile(new CryptoPP::FileSink("../res/publickey"));
        pubkey.DEREncode(publickeyfile);
        publickeyfile.MessageEnd();
        return !0;
    }

private:

};

class Crypto_RSA
{
public:
    Crypto_RSA()
    {

    }

    bool RSA_Crypto(std::vector<byte> data, CryptoPP::CipherDir direction)
    {
        if (direction == CryptoPP::ENCRYPTION)
        {
            CryptoPP::AutoSeededRandomPool rand;
            CryptoPP::ByteQueue byts;
            CryptoPP::FileSource file("../res/publickey", true, new CryptoPP::Base64Decoder);
            file.TransferTo(byts);
            byts.MessageEnd();
            CryptoPP::RSA::PublicKey publickey;
            publickey.Load(byts);
            CryptoPP::RSAES_OAEP_SHA_Encryptor cryptoalg(publickey);
            std::string tmp = "";
            CryptoPP::StringSource(data.data(), data.size(), true, new CryptoPP::PK_EncryptorFilter(rand, cryptoalg, new CryptoPP::StringSink(tmp)));
            std::vector<byte> byt(tmp.begin(), tmp.end());
            _data = byt;
        }
        else
        {
            CryptoPP::AutoSeededRandomPool rand;
            CryptoPP::ByteQueue byts;
            CryptoPP::FileSource file("../res/privatekey", true, new CryptoPP::Base64Decoder);
            file.TransferTo(byts);
            byts.MessageEnd();
            CryptoPP::RSA::PrivateKey privatekey;
            privatekey.Load(byts);
            CryptoPP::RSAES_OAEP_SHA_Decryptor cryptoalg(privatekey);
            std::string tmp = "";
            CryptoPP::StringSource(data.data(), data.size(), true, new CryptoPP::PK_DecryptorFilter(rand, cryptoalg, new CryptoPP::StringSink(tmp)));
            std::vector<byte> byt(tmp.begin(), tmp.end());
            _data = byt;
        }
        return !_data.empty();
    }

    std::vector<byte> &GetData() { return _data; }

private:
    std::vector<byte> _data;
};

int main()
{
    MyFile plaintext;
    KeysGenerator genkey;
    Crypto_RSA rsa;
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
            std::cout << "Please wait..." << std::endl;
            if (!genkey.Generate((plaintext.GetFileSize() + 42) * 8))
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
            if (!rsa.RSA_Crypto(plaintext.GetData(), mode))
            {
                std::cout << _ERROR_ENCRIPT << std::endl;
            }
            else
            {
                plaintext.SetData(rsa.GetData());
                plaintext.Write(std::string("result"));
                std::cout << _OUTPUT_OK << std::endl;
            }
        }

        loop = (command[0] != '5');
        system("pause");
        system("cls");
    }

    return 0;
}