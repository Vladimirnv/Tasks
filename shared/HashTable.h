#include "..\shared\Hash.h"

class HashTable
{
public:
    HashTable()
    {
        _hfunc = false;
    }

    ~HashTable()
    {
        ClearTable();
    }

    bool GetHashFunc()
    {
        return _hfunc;
    }

    void SetHashFunc(bool hfunc)
    {
        _hfunc = hfunc;
    }

    void Create(std::shared_ptr<FileList> &head)
    {
        _file = fopen("../res/result.txt", "wb");
        printf("Creating hashtable...\n");
        ClearTable();
        uint16_t pos = 0;
        do
        {
            if (!_hfunc)
            {
                pos = HashToPos(GetSHAHash(ToVect(head.get()->GetKey())));
            }
            else
            {
                pos = HashToPos(GetMyHash(ToVect(head.get()->GetKey())));
            }
            if (_list[pos].get() != NULL)
            {
                std::weak_ptr<FileList> tmp = _list[pos].get()->GetNext();
                _list[pos].get()->SetNext(std::make_shared<FileList>(head.get()->GetData(), head.get()->GetPath(), head.get()->GetName()));
                _list[pos].get()->GetNext().get()->SetNext(tmp.lock());
                _list[pos].get()->Inc();
                tmp.reset();
                fwprintf(_file, L"     %s\n", _list[pos].get()->GetKey().c_str());
            }
            else
            {
                _list[pos] = std::make_shared<FileList>(head.get()->GetData(), head.get()->GetPath(), head.get()->GetName());
                fwprintf(_file, L"%s\n", _list[pos].get()->GetKey().c_str());
            }
            head = head.get()->GetNext();
        } while (head != NULL);
        fclose(_file);
    }

    double GetAV()
    {
        int count = 0;
        int summ = 0;
        for (int i = 0; i < 65536; i++)
        {
            if (_list[i].get() != NULL)
            {
                summ += _list[i].get()->GetCount();
                count++;
            }
        }
        return summ * 1.0 / count;
    }

    void ClearTable()
    {
        for (int i = 0; i < 65536; i++)
        {
            _list[i].reset();
        }
    }

    int GetCount()
    {
        int count = 0;
        for (int i = 0; i < 65536; i++)
        {
            if (_list[i] != NULL)
            {
                std::weak_ptr<FileList> tmp = _list[i];
                tmp = tmp.lock().get()->GetNext();
                while (tmp.lock() != NULL)
                {
                    count++;
                    tmp = tmp.lock().get()->GetNext();
                }
                tmp.reset();
            }
        }
        return count;
    }

private:
    std::shared_ptr<FileList> _list[65536];
    bool _hfunc;
    FILE* _file;

    std::vector<byte> GetMyHash(std::vector<byte> data)
    {
        MyHash hash;
        hash.GenHash(data, 32);
        return hash.GetData();
    }

    std::vector<byte> GetSHAHash(std::vector<byte> data)
    {
        SHAHash hash;
        hash.GenHash(data);
        return hash.GetData();
    }

    std::vector<byte> ToVect(std::wstring str)
    {
        std::vector<byte> vect;
        vect.assign(str.begin(), str.end());
        return vect;
    }

    uint16_t HashToPos(std::vector<byte> data)
    {
        uint64_t tmp = 0;
        int offset = 0;
        for each (byte item in data)
        {
            tmp += (item << offset);
            offset++;
        }
        return tmp % 65536;
    }
};