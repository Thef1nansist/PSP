#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Winsock2.h"
#include <string>
#include "WS2tcpip.h"
#pragma comment(lib, "WS2_32.lib")

using namespace std;

string GetErrorMsgText(int code) {
    string msgText;
    switch (code) {
    case WSAEINTR: msgText = "Function interrupted";
        break;
    case WSAEACCES: msgText = "Permission denied";
        break;
    case WSAEFAULT: msgText = "Wrong address";
        break;
    case WSASYSCALLFAILURE: msgText = "System call abort";
        break;
    default: msgText = "***ERROR***";
        break;
    }
    return msgText;
}


string SetErrorMsgText(string msgText, int code) { return  msgText + GetErrorMsgText(code); }

int main()
{
    setlocale(LC_ALL, "rus");
    SOCKET cC;
    WSADATA ws;
    SOCKADDR_IN serv;

    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &ws) != 0) {
            throw SetErrorMsgText("Startup: ", WSAGetLastError());
        }

        if (INVALID_SOCKET == (cC = socket(AF_INET, SOCK_STREAM, NULL))) {

            throw SetErrorMsgText("Socket: ", WSAGetLastError());
        }

        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        inet_pton(AF_INET, "127.0.0.1", &(serv.sin_addr));
        if (SOCKET_ERROR == connect(cC, (sockaddr*)&serv, sizeof(serv))) {
            throw SetErrorMsgText("Connect: ", WSAGetLastError());
        }

        char ibuf[50], obuf[50] = "Hello from Client";
        int libuf = 0, lobuf = 0;
        char b[] = "Hello from Client";

        int count;
        cout << "Enter count of messages" << endl;
        cin >> count;

        for (int i = 0; i < count; i++)
        {
            if (SOCKET_ERROR == send(cC, obuf, sizeof(obuf), NULL))
            {
                cout << "Send : " << GetLastError() << endl;;
            }

            if (SOCKET_ERROR == recv(cC, ibuf, sizeof(ibuf), NULL))
            {
                cout << "Recv : " << GetLastError() << endl;
            }
            cout << (i + 1) << "Serv: " << ibuf << endl;
        }

        if (SOCKET_ERROR == send(cC, "CLOSE", sizeof("CLOSE"), NULL)) {
            cout << "send exit: " << GetLastError() << endl;
        }

        if (closesocket(cC) == SOCKET_ERROR) {
            throw SetErrorMsgText("CloseSocket", WSAGetLastError());
        }

        if (WSACleanup() == SOCKET_ERROR) {
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
        }

    }
    catch (string errMsg)
    {
        cout << endl  << errMsg;
    }
    
    system("pause");
    return 0;
}
