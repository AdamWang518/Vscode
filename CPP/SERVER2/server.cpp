#include <iostream>
#include <stdlib.h>  
#include <limits.h>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <regex>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFSIZE 8096
using namespace std;
class Objective{
public:
    Objective(){};
    char  method[512];
    char  filename[512];
    char  version[512];
    bool  hasRange;
    int   size;
    int   start;
};
int BADREQUEST(int socketfd)
{
    const char *sendbuf = "HTTP/1.0 400 BAD REQUEST\nContent-Type: text/html\n\n<!DOCTYPE html><html><head><title>400 BAD REQUEST</title></head><body><h1>400 BAD REQUEST</h1></body></html>";;
    //printf("Send buf to client (0x%x) \n", &sendbuf);
    cout << "400 BAD REQUEST" << endl;
    int iResult;
    //----------------------
    // Send an initial buffer
    iResult = send(socketfd,sendbuf,(int)strlen(sendbuf),0);
    if (iResult<0)
    {
        //terminate the program when send fail with error
        printf("send have failed with error.\n");
        close(socketfd);
        exit(1);
    }
    else
    {
        close(socketfd);
        exit(0);
    }
}
int NOTFOND(int socketfd)
{
    const char *sendbuf = "HTTP/1.0 404 NOT FOUND\nContent-Type: text/html\n\n<!DOCTYPE html><html><head><title>404 NOT FOUND</title></head><body><h1>404 NOT FOUND</h1></body></html>";;
    //printf("Send buf to client (0x%x) \n", &sendbuf);
    cout << "404 NOT FOUND" << endl;
    int iResult;
    //----------------------
    // Send an initial buffer
    iResult = write(socketfd,sendbuf,(int)strlen(sendbuf));
    if (iResult<0)
    {
        //terminate the program when send fail with error
        printf("send have failed with error.\n");
        close(socketfd);
        exit(1);
    }
    else
    {
        close(socketfd);
        exit(0);
    }
}
void parse_request(int fd,char *sizeRequest,Objective& Obj)
{
    string reqMethod = "", target = "",version = "";
    char* range;
    char *request;
    if((range = strstr(sizeRequest, "Range: bytes=")) == NULL){
        cout << "no range in this request." << endl;
        Obj.hasRange = false;
        Obj.start = 0;
    }
    else{
        cout << "find out range in this request." << endl;
        range = range + 13;
        range = strtok(range, "-");
        Obj.hasRange = true;
        string start = range;
        Obj.start = stoi(range);
        cout << "start at: " << Obj.start << endl;
    }
    request = strtok(sizeRequest, " ");//擷取使用的method
    reqMethod += request;
    cout << reqMethod << endl;
    request = strtok(NULL, " ");    //擷取檔名
    if(request == NULL){    //沒有要求，格式錯誤
        BADREQUEST(fd);
    }
    request = request + 1; // 跳過第一個斜槓，取得路徑與檔名
    target += request;
    cout << target << endl;
    request = strtok(NULL, " \r\n");
    if(request == NULL){//沒有版本，格式錯誤
        BADREQUEST(fd);
    }
    version += request;
    cout << version << endl;
    strncpy(Obj.method, reqMethod.c_str(), 20);
    strncpy(Obj.filename, target.c_str(), 200);
    strncpy(Obj.version, version.c_str(), 20);
}
void handle__request(int fd)
{
    Objective obj;
    int sRecv;
    char buffer[4096]={0};
    char filename[50]={0};
    while(1)
    {
        cout << "a connection was found.\n";
        sRecv=read(fd, buffer, sizeof(buffer));
        cout << buffer << endl;
        parse_request(fd, buffer, obj);
        if(strcmp(obj.method, "GET")){
            cout << "wrong method" << endl;
            BADREQUEST(fd);
        }
        if(strcmp(obj.version,"HTTP/1.1"))
        {
            cout << "wrong version" << endl;
            BADREQUEST(fd);
        }
        char contentType[30];
        if(strstr(obj.filename, ".mp4") != NULL){
            cout << "A Video request" << endl;
            strcpy(contentType, "video/mp4");
        }
        else if(strstr(obj.filename, ".html") != NULL){
            cout << "A Text request" << endl;
            strcpy(contentType, "text/html");
        }
        char Resource[4096]={0};
        if(strcmp(contentType, "video/mp4") == 0)
        {
            FILE *reader = fopen(obj.filename, "rb+");//讀取影片檔案
            if(reader == NULL){
                cout << obj.filename << ":does not exist" << endl;
                NOTFOND(fd);
                exit(1);     
            }
            else
            {
                cout << obj.filename << ":does  exist" << endl;
                fseek(reader, 0L, SEEK_END);
                int fileLength = ftell(reader);
                fseek(reader, 0, SEEK_SET);
                if(!obj.hasRange){  // 第一次先送header跟range
                    cout << "200 OK" << endl;
                    snprintf(Resource, 4096, "HTTP/1.1 200 OK\nContent-Type: %s\nContent-Length: %d\nAccept-Ranges: bytes\n\n", contentType, fileLength);
                    cout << Resource << endl;
                    send(fd, Resource, strlen(Resource), MSG_NOSIGNAL);
                }
                else//以片段傳送後續影片
                {
                    long contentLeft = fileLength - obj.start;
                    int fragSize = 1024*64;
                    int fragNum = contentLeft / fragSize;
                    if(contentLeft % fragSize != 0)
                    {
                        fragNum++;
                    }
                    for(int i = 0; i < fragNum; i++)
                    {
                        if(i+1 == fragNum)
                        {
                            fragSize = fileLength - obj.start;
                        }
                        snprintf(Resource, 4096, "HTTP/1.1 206 Partial Content\nContent-Type: %s\nContent-Length: %d\nContent-Range: bytes %d-%d/%d\nAccept-Ranges: bytes\n\n",contentType, fragSize, obj.start, obj.start+fragSize-1, fileLength);
                        send(fd, Resource, strlen(Resource), MSG_NOSIGNAL);
                        char sizeBuffer[fragSize]={0};
                        fseek(reader, obj.start, SEEK_SET);
                        if(fread(sizeBuffer, 1, fragSize, reader) == 0){
                            cout << "read video file error." << endl;
                            exit(1);
                        }
                        else
                        {
                            send(fd, sizeBuffer, sizeof(sizeBuffer), MSG_NOSIGNAL);
                            obj.start += fragSize;
                            memset(sizeBuffer, 0, sizeof(sizeBuffer));
                        }
                    }
                    cout << "Transfer video done" << endl;
                }
            }
        }
        else
        {
            FILE *reader = fopen(obj.filename, "r");
            if(reader == NULL){
                cout << obj.filename << ":does not exist" << endl;
                NOTFOND(fd);
                exit(1);     
            }
            else
            {
                cout << obj.filename << ":does  exist" << endl;
                fseek(reader, 0L, SEEK_END);
                int fileLength = ftell(reader);
                fseek(reader, 0, SEEK_SET);
                
                snprintf(Resource, 4096, "HTTP/1.1 200 OK\nContent-Type: %s\nContent-Length: %d\nAccept-Ranges: bytes\n\n", contentType, fileLength);
                send(fd, Resource, strlen(Resource), MSG_NOSIGNAL);
                cout << "header 200 OK" << endl;
                char siezBuffer[fileLength]={0};
                if(fread(siezBuffer, 1, fileLength, reader) == 0){
                    cout << "error.\n";
                    exit(1);
                }
                send(fd, siezBuffer, sizeof(siezBuffer), 0);
                cout << "sending body\n";
            }
        }
    }
    
    BADREQUEST(fd);
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
        exit(1);
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
        exit(1);
    }
    else
    {
        cout << "Bind Success.\n";
    }
    if (listen(listenfd,64)<0)
    {
        cout << "Listen Fail.\n";
        exit(1);
    }
    else
    {
        cout << "Listen Success.\n";
    }
    while(1) {
        cout << "Waiting for connection... "<<endl;
        length = sizeof(cli_addr);
        /* 等待客戶端連線 */
        if ((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length))<0)
        {
            cout << "Accept Fail.\n";
            exit(1);
        }
        /*連線成功*/
        int id = fork();
        if(id == -1){
            cout << "fork error.\n";//fork失敗
            return -1;
        }
        if(id == 0){   // 子程序
            handle__request(socketfd);
            exit(0);
        }
        else if(id > 0){
            close(socketfd);
        }
    }    
    return 0;
}