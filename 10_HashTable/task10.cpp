#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <tchar.h>
#include <Windows.h>
#include <algorithm>
#include "..\shared\FileList.h"
#include "..\shared\HashTable.h"

int main()
{
    std::shared_ptr<FileList> filelist = std::make_shared<FileList>();
    std::shared_ptr<FileList> head = filelist;
    //C:/Windows/System32C:/Windows/SysWOW64D:/Database
    std::wstring path = _T("C:/Windows/System32");
    printf("Data loading. Please wait.\n");
    ScanFolder((LPTSTR)path.c_str(), (LPCTSTR)_T("*"), filelist);
    std::string command = "";
    double AVmax = 0;
    double time = 0;
    clock_t start, end;
    do
    {
        wprintf(_T("Data loaded from %s\n"), path.c_str());
        printf("AVmax: ");
        std::cin >> AVmax;
        printf("MyHash: 1    SHAHash: 0\n");
        printf(">> ");
        scanf("%s", command.c_str());
        std::shared_ptr<FileList> temp = head;
        HashTable hashtable;
        hashtable.SetHashFunc(command[0] == '1');
        start = clock();
        hashtable.Create(temp);
        end = clock();
        printf("Working time:  %.2fs\n", (end - start) * 1.0 / CLOCKS_PER_SEC);
        printf("AV = %.4f\n", hashtable.GetAV());
        if (hashtable.GetAV() > AVmax)
        {
            printf("AV(%.4f) > AV(%.4f)\nRecreating...\n", hashtable.GetAV(), AVmax);
            temp = head;
            hashtable.SetHashFunc(!hashtable.GetHashFunc());
            start = clock();
            hashtable.Create(temp);
            end = clock();
            printf("Working time:  %.2fs\n", (end - start) * 1.0 / CLOCKS_PER_SEC);
            printf("AV = %.4f\n", hashtable.GetAV());
        }
        printf("Input e for exit or other to repeat\n");
        printf(">> ");
        scanf("%s", command.c_str());
        temp.reset();
        system("cls");
    } while (command[0] != 'e');
    do
    {
        head = head.get()->GetNext();
    } while (head.get()->GetNext() != NULL);
    filelist.reset();
    head.reset();
    printf("Please wait. Cleaning. The program end automatically.\n");
    return 0;
}