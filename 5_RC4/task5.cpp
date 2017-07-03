#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "..\shared\MyFile.h"
#include "..\shared\KeysGenerator.h"

#define byte unsigned char

#define _MENU "Menu: "
#define _MENU_ONE "1: Chose input file"
#define _MENU_TWO "2: Generate key"
#define _MENU_THREE "3: Encript file"
#define _MENU_FOUR "4: Exit"
#define _INPUT_DIALOG "Input path: "
#define _OUTPUT_DIALOG "Output path: "
#define _ERROR_OPEN "Can not open file"
#define _ERROR_ENCRIPT "Can not encript file"
#define _OUTPUT_OK "Done!"

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

int main()
{
    MyFile plaintext;
    MyFile keytext;
    KeysGenerator genkey;
    Crypto_RC4 rc4;
    bool loop = true;
    std::string command = "";
    std::string path = "";
    while (loop)
    {
        std::cout << _MENU << std::endl << _MENU_ONE << std::endl << _MENU_TWO << std::endl << _MENU_THREE << std::endl << _MENU_FOUR << std::endl;
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
            srand(time(NULL));
            if (!genkey.Generate(rand() % 100 + 10))
            {
                std::cout << _ERROR_OPEN << std::endl;
            }
            else
            {
                std::cout << _OUTPUT_OK << std::endl;
            }
            break;
        case '3':
            std::cout << "Keyfile " << _INPUT_DIALOG;
            std::cin >> path;
            if (!keytext.Open(path))
            {
                std::cout << _ERROR_OPEN << std::endl;
            }
            else
            {
                if (!rc4.RC4_Crypto(plaintext.GetData(), keytext.GetData(), 8))
                {
                    std::cout << _ERROR_ENCRIPT << std::endl;
                }
                else
                {
                    plaintext.SetData(rc4.GetData());
                    plaintext.Write(std::string("result"));
                    std::cout << _OUTPUT_OK << std::endl;
                }
            }
            break;

        default:
            break;
        }

        loop = (command[0] != '4');
        system("pause");
        system("cls");
    }

    return 0;
}
