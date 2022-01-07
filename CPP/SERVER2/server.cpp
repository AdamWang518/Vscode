#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFSIZE 8096
using namespace std;
int BADREQUEST(int socketfd)
{
    const char *sendbuf = "HTTP/1.0 400 BAD REQUEST\r\nContent-Type: text/html\r\n\r\n<style>body{background: #ffffff;margin: 0;}</style>400 BAD REQUEST";
    //printf("Send buf to client (0x%x) \n", &sendbuf);
    int iResult;
    //----------------------
    // Send an initial buffer
    iResult = send(socketfd,sendbuf,(int)strlen(sendbuf),0);
    if (iResult<0)
    {
        //terminate the program when send fail with error
        printf("send have failed with error.\n");
        close(socketfd);
        return 1;
    }
    else
    {
        return 0;
    }
}
void handle_socket(int fd)
{
    

}
int main()
{
    char YN;
    int PORT_NUM;
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
            PORT_NUM = 80;
            break;
        }
        else
        {
            continue;
        }
    }
    int i, pid, listenfd, socketfd, sRecv;
    socklen_t length;
    static struct sockaddr_in cli_addr;
    static struct sockaddr_in serv_addr;
    if ((listenfd=socket(AF_INET, SOCK_STREAM,0))<0)
    {
        cout<<"Fail to create a socket.\n";
        exit(3);
    }
    else
    {
        cout << "create socket success.\n";
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT_NUM);
    if (bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    {
        cout << "Bind Fail.\n";
        exit(3);
    }
    else
    {
        cout << "Bind Success.\n";
    }
    if (listen(listenfd,64)<0)
    {
        cout << "Listen Fail.\n";
        exit(3);
    }
    else
    {
        cout << "Listen Success.\n";
    }
    while(1) {
        cout << "Waiting for connection... "<<endl;
        char buffer[4096]={0};
        char filename[50] = {0};
        length = sizeof(cli_addr);
        /* 等待客戶端連線 */
        if ((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length))<0)
        {
            cout << "Accept Fail.\n";
            exit(3);
        }
        else
        {
            /*連線成功*/
            cout << "a connection was found.\n";
            sRecv=recv(socketfd, buffer, sizeof(buffer), 0);
            cout << buffer << endl;
            /* 分出子行程處理要求 */
            if ((pid = fork()) < 0) {
                cout << "Fork Fail.\n";
                exit(3);
            }
            else
            {
                
            }
        }
        
    }    
    return 0;
}