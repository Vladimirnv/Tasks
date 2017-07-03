#include <tchar.h>
#include "windows.h"
#include "tlhelp32.h"
#include <psapi.h>
#include <map>

class ProcessList
{
public:
    ProcessList()
    {
        GetProcList();
        _proclist = _proclist_new;
    };

    void Update()
    {
        if (!GetProcList())
        {

        }
        else
        {
            for each (std::pair<DWORD, std::wstring> item in _proclist_start)
            {
                _tprintf(TEXT("start: %s    PID: %u    Path: %s\n"), item.second.data(), item.first, GetProcessInfo(item.first).c_str());
            }
            for each (std::pair<DWORD, std::wstring> item in _proclist_stop)
            {
                _tprintf(TEXT("stop: %s    PID: %u    Path: %s\n"), item.second.data(), item.first, GetProcessInfo(item.first).c_str());
            }
        }
    }

    std::map<DWORD, std::wstring> Compaire(std::map<DWORD, std::wstring> news, std::map<DWORD, std::wstring> old)
    {
        std::map<DWORD, std::wstring> start = news;
        for each (std::pair<DWORD, std::wstring> item in old)
        {
            start.erase(item.first);
        }

        return start;
    }

    std::map<DWORD, std::wstring> Summ(std::map<DWORD, std::wstring> old, std::map<DWORD, std::wstring> news)
    {
        std::map<DWORD, std::wstring> current = old;
        for each (std::pair<DWORD, std::wstring> item in news)
        {
            current.insert(item);
        }

        return current;
    }

    bool GetProcList()
    {
        _proclist_new.clear();
        _snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (_snapshot == INVALID_HANDLE_VALUE)
        {
            return false;
        }
        _proc.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(_snapshot, &_proc))
        {
            do
            {
                _proclist_new.insert(std::pair<DWORD, std::wstring>(_proc.th32ProcessID, _proc.szExeFile));
            } while (Process32Next(_snapshot, &_proc));
        }

        _proclist_start = Compaire(_proclist_new, _proclist);
        _proclist_stop = Compaire(_proclist, _proclist_new);
        _proclist = Compaire(Summ(_proclist, _proclist_start), _proclist_stop);

        return true;
    }

    std::wstring GetProcessInfo(DWORD id)
    {
        TCHAR procpath[MAX_PATH] = TEXT("unknown path");
        HANDLE prochandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, id);
        if (NULL != prochandle)
        {
            GetModuleFileNameEx(prochandle, NULL, procpath, sizeof(procpath) / sizeof(TCHAR));
        }
        CloseHandle(prochandle);

        return procpath;
    }

private:
    HANDLE _snapshot;
    PROCESSENTRY32 _proc;
    std::map<DWORD, std::wstring> _proclist;
    std::map<DWORD, std::wstring> _proclist_new;
    std::map<DWORD, std::wstring> _proclist_start;
    std::map<DWORD, std::wstring> _proclist_stop;
};

int main()
{
    ProcessList pl;
    while (true)
    {
        pl.Update();
        Sleep(500);
    }
    system("pause");
    return 0;
}
