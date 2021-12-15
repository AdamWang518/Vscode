#include <stdio.h>
#include <winsock2.h>
#include<iostream>
#pragma comment (lib, "Ws2_32.lib")  //載入 ws2_32.dll
using namespace std;
void Find(SOCKET clntSock)
{
    cout << "Find" << endl;
    char *str = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>example html<h1></body></html>";  
    send(clntSock, str, (int)strlen(str)+sizeof(char), NULL);
}
void notFind(SOCKET clntSock)
{
    cout << "Not find" << endl;
    char *str = "HTTP1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n<html><body><h1>404 not found<h1></body></html>";  
    send(clntSock, str, (int)strlen(str)+sizeof(char), NULL);
}
int main(){
    int port;
    cout << "set port:";
    cin >> port;
    //初始化 DLL
    WSADATA wsaData;  
    WSAStartup( MAKEWORD(2, 2), &wsaData);  
    //建立套接字
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  
    //繫結套接字
    sockaddr_in sockAddr;  
    memset(&sockAddr, 0, sizeof(sockAddr));  //每個位元組都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具體的IP地址
    sockAddr.sin_port = htons(port);  //埠
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));  
    //進入監聽狀態
    listen(servSock, 20);  
    //接收客戶端請求
    SOCKADDR clntAddr;  
    int nSize = sizeof(SOCKADDR);  
    SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
    Find(clntSock);
    //向客戶端傳送資料    
    //關閉套接字
    closesocket(clntSock);  
    closesocket(servSock);  
    //終止 DLL 的使用
    WSACleanup();  
    return 0;  
}