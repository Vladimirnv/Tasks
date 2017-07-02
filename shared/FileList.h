#include <cstdlib>
#include <iostream>
#include <memory>
#include <Windows.h>

//#define DEBUG

class FileList
{ 
#ifdef DEBUG
static int count(const int  deltha = 0)
    {
        static int  counter = 0;
        return  counter += deltha;
    }
#endif // DEBUG

public:
    FileList()
    {
        _next.reset();
        _count = 1;
#ifdef DEBUG
        count(1);
#endif // DEBUG
    }

    FileList(WIN32_FIND_DATA data,
    std::wstring path = _T(""),
    std::wstring name = _T(""))
    {
        _data = data;
        _path = path;
        _name = name;
        _next.reset();
        _count = 1;
#ifdef DEBUG
        count(1);
#endif // DEBUG
    }

    ~FileList()
    {
#ifdef DEBUG
        count(-1);
        printf("%d\n", count());
#endif // DEBUG
    }

    std::wstring GetKey() { return _path + _name; }

    std::shared_ptr<FileList> GetNext() 
    {
        return _next;
    }

    void SetNext(std::shared_ptr<FileList> next) 
    {
        _next = next;
    }

    WIN32_FIND_DATA GetData()
    {
        return _data;
    }

    std::wstring GetPath()
    {
        return _path;
    }

    std::wstring GetName()
    {
        return _name;
    }

    void Inc()
    {
        _count++;
    }

    int GetCount()
    {
        return _count;
    }

private:
    WIN32_FIND_DATA _data;
    std::wstring _path;
    std::wstring _name;
    int _count;
    std::shared_ptr<FileList> _next;
};

void ScanFolder(LPTSTR folder, LPCTSTR mask, std::shared_ptr<FileList> &fl)
{
    WIN32_FIND_DATA atr;
    HANDLE hFind;
    TCHAR nfolder[_MAX_PATH];
    lstrcpy(nfolder, folder);
    lstrcat(nfolder, _T("/"));
    lstrcat(nfolder, mask);
    hFind = FindFirstFile(nfolder, &atr);
    do
    {
        if (!(atr.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            fl.get()->SetNext(std::make_shared<FileList>(atr, folder, atr.cFileName));
            fl = fl.get()->GetNext();
        }
        else
        {
            if (_tcscmp(atr.cFileName, _T(".")) && _tcscmp(atr.cFileName, _T("..")))
            {
                TCHAR dir[_MAX_PATH];
                lstrcpy(dir, folder);
                lstrcat(dir, _T("/"));
                lstrcat(dir, atr.cFileName);
                ScanFolder(dir, mask, fl);
            }
        }
    } while (FindNextFile(hFind, &atr));

    FindClose(hFind);
}