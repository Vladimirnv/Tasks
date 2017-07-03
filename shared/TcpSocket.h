#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <windows.h>

#pragma (lib, "..\third-party\lib\WS2_32.lib")

#define TRYCOUNT 3
#define TRYTIME 1000

class TcpSocket
{
public:
    TcpSocket(bool isserver = false)
    {
        Init();
        if (isserver)
        {
            _isServer = true;
            SetServer();
            ServerRequest(); 
        }
        else
        {
            _isServer = false;
            SetClient();
            Connect();
            if (!TryHello())
            {
                _isServer = true;
                Close();
                Init();
                SetServer();
                ServerRequest();
            }
        }
    }

    void Send(char* str)
    {
        int total = strlen(str);
        int sent = 0;
        int bytes = 0;
        do
        {
            bytes = send(_current, str + sent, total - sent, 0);
            sent += bytes;
        } while ((sent < total) && (bytes != 0) && (bytes != -1));
        std::cout << "Send: " << str << std::endl;
    }

    char* Receive()
    {
        char tmp[2048] = "";
        recv(_current, &tmp[0], sizeof(tmp), 0);
        std::cout << "Received: " << tmp << std::endl;
        return tmp;
    }

    void SearchClient()
    {
        if (_isServer)
        {
            AcceptConnection();
            ServerRequest();
        }
    }

    bool IsServer()
    {
        return _isServer;
    }

    void Close()
    {
        closesocket(_socket);
        closesocket(_current);
        WSACleanup();
    }

    ~TcpSocket()
    {
        Close();
    }
private:
    bool _isServer;
    WORD _versionrequest;
    WSADATA _wdata;
    SOCKET _socket;
    SOCKET _current;
    struct sockaddr_in _address;
    u_short _port;

    void ServerRequest()
    {
        char* message;
        bool con = false;
        if (_isServer)
        {
            do
            {
                message = Receive();
                con = strcmp(message, "Hello! I'm client!") == 0;
                if (con)
                    Send("Hello! I'm server!");
                else 
                    SearchClient();
                Sleep(1000);
            } while (!con);
        }
    }

    void Init()
    {
        _port = 80;
        _versionrequest = MAKEWORD(2, 2);
        WSAStartup(_versionrequest, &_wdata);
        _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    void SetPorts()
    {
        _address.sin_family = AF_INET;
        _address.sin_port = htons(_port);
        closesocket(_current);
    }

    void SetClient()
    {
        SetPorts();
        _address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    }

    void SetServer()
    {
        SetPorts();
        _address.sin_addr.s_addr = INADDR_ANY;
        Listening();
    }

    void Listening()
    {
        bind(_socket, (LPSOCKADDR)&_address, sizeof(_address));
        listen(_socket, SOMAXCONN);
        AcceptConnection();
    }

    bool TryHello()
    {
        char* tmp;
        int i = 0;
        bool ok = false;
        do
        {
            Send("Hello! I'm client!");
            tmp = Receive();
            i++;
            ok |= strcmp(tmp, "Hello! I'm server!") == 0;
            Sleep(TRYTIME);
        } while ((i < TRYCOUNT) && !ok);
        return ok;
    }

    void Connect()
    {
        connect(_socket, (struct sockaddr *)&_address, sizeof(struct sockaddr));
        _current = _socket;
    }

    void AcceptConnection()
    {
        printf("Waiting for client...\n");
        sockaddr_in from;
        int fromlen = sizeof(from);
        _current = accept(_socket, (struct sockaddr*)&from, &fromlen);
        printf("Connected!\n");
    }
};