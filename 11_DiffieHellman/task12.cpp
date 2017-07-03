#include "..\shared\KeyExchanger.h"
#include "..\shared\TcpSocket.h"
#include "..\shared\RC4.h"

void ServerAck(TcpSocket *s, KeyExchanger *k)
{
    char *message = "";
    message = s->Receive();
    k->SetG(message);
    s->Send("OK");
    message = s->Receive();
    k->SetP(message);
    s->Send("OK");
    k->GenPP();
    message = s->Receive();
    k->GenK(message);
    message = k->GetA();
    s->Send(message);
    message = k->GetKey();
    std::cout << "Generated key: " << message << std::endl;
    system("pause");
    system("cls");
}

void ClientAck(TcpSocket *s, KeyExchanger *k)
{
    char *message = "";
    k->First();
    message = k->GetG();
    s->Send(message);
    s->Receive();
    message = k->GetP();
    s->Send(message);
    s->Receive();
    k->GenPP();
    message = k->GetA();
    s->Send(message);
    message = s->Receive();
    k->GenK(message);
    message = k->GetKey();
    std::cout << "Generated key: " << message << std::endl;
    system("pause");
    system("cls");
}

char* Encript(char t[2048], KeyExchanger *k)
{
    Crypto_RC4 rc4;
    char str[2048];
    char *message = "";
    std::vector<byte> text(t, t + strlen(t));
    message = k->GetKey();
    strcpy(str, message);
    std::vector<byte> key(str, str + strlen(str));
    rc4.RC4_Crypto(text, key, 8);
    text = rc4.GetData();
    memset(str, 0, sizeof str);
    memcpy(str, &text[0], text.size());
    return str;
}

int main()
{
    TcpSocket sock;
    KeyExchanger keyexchanger;
    char str[2048];
    char *message = "";
    if (sock.IsServer())
    {
        printf("I'm server.\n");
        ServerAck(&sock, &keyexchanger);
        while (true)
        {
            str[0] = 0;
            message = sock.Receive();
            strcpy(str, message);
            if (str[0] != 0)
            {
                message = Encript(str, &keyexchanger);
                std::cout << "Received encripted: " << message << std::endl;
                strcpy(str, message);
                message = Encript(str, &keyexchanger);
                sock.Send(message);
            }
            else
            {
                sock.SearchClient();
                ServerAck(&sock, &keyexchanger);
            }
        }
    }
    else
    {
        printf("I'm client.\n");
        ClientAck(&sock, &keyexchanger);
        while (true)
        {
            printf(">> ");
            std::cin >> str;
            message = Encript(str, &keyexchanger);
            sock.Send(message);
            message = sock.Receive();
            strcpy(str, message);
            message = Encript(str, &keyexchanger);
            std::cout << "Received encripted: " << message << std::endl;
        }
    }
    system("pause");
    return 0;
}

