#include "iostream"
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "..\shared\MyFile.h"
#include "..\shared\KeysGenerator.h"

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

std::vector<bytes> Encript(std::vector<bytes> data, std::vector<bytes> &ktext)
{
    std::vector<bytes> result;
    result.resize(data.size());
    for (int i = 0; i < data.size(); i++)
    {
        result.at(i) = data.at(i) ^ ktext.at(i);
    }
    return result;
}

int main()
{
	MyFile plaintext;
	MyFile keytext;
	KeysGenerator genkey;
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
                std::cout << "Keyfile " << _INPUT_DIALOG;
                std::cin >> path;
                if (!keytext.Open(path))
                {
                    std::cout << _ERROR_OPEN << std::endl;
                }
                else
                {
                    plaintext.SetData(Encript(plaintext.GetData(), keytext.GetData()));
                    if (plaintext.GetData().empty())
                    {
                        std::cout << _ERROR_ENCRIPT << std::endl;
                    }
                    else
                    {
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