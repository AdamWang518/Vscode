#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <cstdio>
#include <fstream>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
int GET(SOCKET sConnection)
{
    const char *sendbuf = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<style>body{background: #ffffff;margin: 0;}</style>Hello, world!This is My Test Message!";
    printf("Send buf to client (0x%x) \n", &sendbuf);
    int iResult;
    //----------------------
    // Send an initial buffer
    iResult = send(sConnection,sendbuf,(int)strlen(sendbuf),0);
    if (iResult == SOCKET_ERROR)
    {
        //terminate the program when send fail with error
        printf("send have failed with error :%d \n", WSAGetLastError());
        closesocket(sConnection);
        WSACleanup();
        return 1;
    }
    else
    {
        return 0;
    }
}
int EXIST(SOCKET sConnection,char *filename)
{
    char file[4096] = {0}; //如果有該檔案的話用來放檔案內容
    FILE *fp;
    fp = fopen(filename, "r");
    return GET(sConnection);
}
int BADREQUEST(SOCKET sConnection)
{
    const char *sendbuf = "HTTP/1.0 400 BAD REQUEST\r\nContent-Type: text/html\r\n\r\n<style>body{background: #ffffff;margin: 0;}</style>400 BAD REQUEST";
    printf("Send buf to client (0x%x) \n", &sendbuf);
    int iResult;
    //----------------------
    // Send an initial buffer
    iResult = send(sConnection,sendbuf,(int)strlen(sendbuf),0);
    if (iResult == SOCKET_ERROR)
    {
        //terminate the program when send fail with error
        printf("send have failed with error :%d \n", WSAGetLastError());
        closesocket(sConnection);
        WSACleanup();
        return 1;
    }
    else
    {
        return 0;
    }
}
int NOTFOND(SOCKET sConnection)
{
    const char *sendbuf = "HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n<style>body{background: #ffffff;margin: 0;}</style>404 NOT FOUND";
    printf("Send buf to client (0x%x) \n", &sendbuf);
    int iResult;
    //----------------------
    // Send an initial buffer
    iResult = send(sConnection,sendbuf,(int)strlen(sendbuf),0);
    if (iResult == SOCKET_ERROR)
    {
        //terminate the program when send fail with error
        printf("send have failed with error :%d \n", WSAGetLastError());
        closesocket(sConnection);
        WSACleanup();
        return 1;
    }
    else
    {
        return 0;
    }
}
int main()
{
    char YN;
    int PORT_NUM = 80;
    //set default port number=80
    while(1)
    {
        cout << "Do you want to set port number?Y/N?";
        cin >> YN;
        if(YN=='Y')
        {
            cout << "Please input port number:";
            cin >> PORT_NUM;
            break;
        }
        else if(YN=='N')
        {
            break;
        }
        else
        {
            continue;
        }
    }
    WSADATA wsaData;
    WORD    DLLVersion;
    DLLVersion = MAKEWORD(2,1);//winsocket-dll version
    // 用 WSAStartup 開始 Winsocket-DLL
    int err = WSAStartup(DLLVersion,&wsaData);
    if (err!=0)
    {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n",err);
        return 1;
    }
    if (LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=1)
    {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return 1;
    }
    else
    {
        printf("The Winsock 2.1 dll was found okay\n");
    }
    SOCKADDR_IN    addr;
    int addrlen = sizeof(addr);

    // Create socket
    SOCKET sListen        ;        //listening for an incoming connection
    SOCKET sConnection    ;        //oerating if a connection was found
    SOCKET sRecv;//deal with recv

    // AF_INET        :  表示建立的Socket屬於internet family
    // SOCK_STREAM    :  表示建立的socket是connection-oriented socket
    sConnection = socket(AF_INET,SOCK_STREAM,NULL);

    // 設定位址資訊的資料
    addr.sin_addr.s_addr    = inet_addr("127.0.0.1");
    addr.sin_family            = AF_INET;
    addr.sin_port            = htons(PORT_NUM);


    // 設定Listen
    sListen = socket(AF_INET,SOCK_STREAM,NULL);
    if (sListen == INVALID_SOCKET)
    {
        printf("socket function failed with error : %u \n",WSAGetLastError());
        WSACleanup();
        return 1;
    }
    // Bind the socket
    int iResult = bind (sListen,(SOCKADDR*)&addr,sizeof(addr));
    if (iResult == SOCKET_ERROR)
    {
        printf("Bind failed with error : %u \n", WSAGetLastError());
        closesocket(sListen);
        WSACleanup();
        return 1;
    }
    //SOMAXCONN: listening without any limit
    if(listen(sListen, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen function failed with error: %d \n", WSAGetLastError());
        closesocket(sListen);
        WSACleanup();
        return 1;

    }
    else
        printf("Listening on socket...\n");

 
    // 等待連線
    SOCKADDR_IN clientAddr;
    while (1)
    {
        int result;
        char buffer[4096]={0};
        char filename[50] = {0};
        cout << "Waitting for connect... "<<endl;
        if(sConnection = accept(sListen,(SOCKADDR*)&clientAddr,&addrlen))
        {
            cout << "a connection was found."<<endl;
            sRecv=recv(sConnection, buffer, sizeof(buffer), 0);
            cout << buffer << endl;
            for(int i=4;i<4096;i++) {           
                if(buffer[i] == ' ') {
                    break;
                }
                filename[i - 4] = buffer[i];
            }//分離出filename
            cout << filename << endl;
            printf("Server : got a connection from : %s\n",inet_ntoa(addr.sin_addr));
            if (strncmp(buffer,"GET ",4)&&strncmp(buffer,"get ",4))//若request不是get則回傳bad request
            {
                result = BADREQUEST(sConnection);
            }
            else
            {
                result = EXIST(sConnection, filename);
            }
            if(result==1)
            {
                return 1;
            }
            //Send message to client 
                        
        }
    }
 
    getchar();
    getchar();
}