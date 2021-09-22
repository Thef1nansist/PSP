#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include <ctime>

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
    cout << "Server is started\n";
    cout << "Server is waiting connections\n";
    int count = 0;
    char countBuf[50];
    char oBuf[50] = "ECHO: Hi from client";

    WSADATA ws;
    SOCKET s;
    SOCKET c;

    try
    {
        if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws))) {
            cout << "Socket: " << WSAGetLastError() << endl;
        }
        if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0))) {
            cout << "Socket: " << WSAGetLastError() << endl;
        }


        sockaddr_in s_adr;
        sockaddr_in c_adr;
        {
            s_adr.sin_port = htons(2000);
            s_adr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
            s_adr.sin_family = AF_INET;
        }
        
        if (SOCKET_ERROR == (bind(s, (LPSOCKADDR)&s_adr, sizeof(s_adr)))) {
            cout << "Bind: " << WSAGetLastError() << endl;
        }
        if (SOCKET_ERROR == listen(s, 2)) {
            cout << "Listen: " << WSAGetLastError() << endl;
        }

        for (;;) {
            int lcInt = sizeof(c_adr);
            if (INVALID_SOCKET == (c = accept(s, (sockaddr*)&c_adr, &lcInt)))
            {
                cout << "Accept: " << WSAGetLastError() << endl;
            }

            cout << "    Client connect\n";
            cout << "Addres client:       " << inet_ntoa(c_adr.sin_addr) << " : " << htons(c_adr.sin_port) << endl << endl << endl << endl << endl << endl;

            for (;;) {
                if (SOCKET_ERROR == recv(c, countBuf, sizeof(countBuf), NULL)) {
                    cout << "Recv: " << WSAGetLastError() << endl;
                    break;
                }

                cout << count << "Client: " << countBuf << endl;
                count++;

                if (!strcmp(countBuf, "CLOSE")) {
                    break;
                }
                if (SOCKET_ERROR == send(c, oBuf, strlen(oBuf) + 1, NULL)) {
                    cout << "Send: " << WSAGetLastError() << endl;
                    break;
                }
                count = 0;
                cout << "\t\tClient Disconnect: \n";
            }
            if (closesocket(c) == SOCKET_ERROR)
            {
                throw SetErrorMsgText("closesocket: ", WSAGetLastError());
            }

            if (WSACleanup() == SOCKET_ERROR)
            {
                throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
            }
        }
    }

    catch (string errMassage)
    {
        cout << endl << errMassage;
    }

    return 0;
}
