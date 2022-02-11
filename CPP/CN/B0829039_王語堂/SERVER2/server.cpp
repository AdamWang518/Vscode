#include <iostream>
#include <stdlib.h>  
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
class Objective{
public:
    bool  Range;//儲存是否為range request
    int   startPoint;//檔案讀取的開始點
    char  requestMethod[32];//儲存該request的method
    char  requestFilename[1024];//儲存該request要求的filename
    char  requestVersion[32];//儲存該request的version
};
int BADREQUEST(int socketfd)
{
    const char *sendbuf = "HTTP/1.0 400 BAD REQUEST\nContent-Type: text/html\n\n<!DOCTYPE html><html><head><title>400 BAD REQUEST</title></head><body><h1>400 BAD REQUEST</h1></body></html>";
    //cout << "400 BAD REQUEST" << endl;
    //----------------------
    // Send an initial buffer
    send(socketfd,sendbuf,(int)strlen(sendbuf),0);
    close(socketfd);
    exit(0);

}
int NOTFOND(int socketfd)
{
    const char *sendbuf = "HTTP/1.0 404 NOT FOUND\nContent-Type: text/html\n\n<!DOCTYPE html><html><head><title>404 NOT FOUND</title></head><body><h1>404 NOT FOUND</h1></body></html>";
    //cout << "404 NOT FOUND" << endl;
    //----------------------
    // Send an initial buffer
    write(socketfd,sendbuf,(int)strlen(sendbuf));
    close(socketfd);
    exit(0);
}
void request_parser(int fd,char *sizeRequest,Objective& Obj)
{
    string reqMethod = "", target = "",version = "";
    char* range;
    char *request;
    if((range = strstr(sizeRequest, "Range: bytes=")) == NULL){
        cout << "no range in this request." << endl;//儲存該request沒有range的事實
        Obj.Range = false;
        Obj.startPoint = 0;
    }
    else{
        cout << "find out range in this request." << endl;
        range += 13; //將range指向第一個數字
        range = strtok(range, "-");
        Obj.Range = true;
        string start = range;//儲存該request有range的事實
        Obj.startPoint = stoi(range);
        cout << "start at: " << Obj.startPoint << endl;
    }
    request = strtok(sizeRequest, " ");//擷取使用的method
    reqMethod += request;
    //cout << reqMethod << endl;
    request = strtok(NULL, " ");    //擷取檔名
    if(request == NULL){    //沒有要求，格式錯誤
        BADREQUEST(fd);
    }
    request = request + 1; // 跳過第一個斜槓，取得路徑與檔名
    target += request;
    //cout << target << endl;
    request = strtok(NULL, " \r\n");
    if(request == NULL){//沒有版本，格式錯誤
        BADREQUEST(fd);
    }
    version += request;
    //cout << version << endl;
    strcpy(Obj.requestMethod, reqMethod.c_str());
    strcpy(Obj.requestFilename, target.c_str());
    strcpy(Obj.requestVersion, version.c_str());
}
void request_handler(int fd)
{
    Objective obj;
    char buffer[4096]={0};
    while(1)
    {
        cout << "a connection was found.\n";//收到request
        memset(buffer, 0, sizeof(buffer));//清空buffer
        read(fd, buffer, sizeof(buffer));//將request儲存於buffer
        cout << buffer << endl;
        request_parser(fd, buffer, obj);
        if(strcmp(obj.requestMethod, "GET")){//非GET，格式錯誤，回傳400
            cout << "wrong method" << endl;
            BADREQUEST(fd);
        }
        if(strcmp(obj.requestVersion,"HTTP/1.1"))//版本錯誤，回傳400
        {
            cout << "wrong version" << endl;
            BADREQUEST(fd);
        }
        char contentType[30];
        if(strstr(obj.requestFilename, ".mp4") != NULL){//要求檔案類型為mp4，設定content type為video/mp4
            cout << "A Video request" << endl;
            strcpy(contentType, "video/mp4");
        }
        else if(strstr(obj.requestFilename, ".html") != NULL){//要求檔案類型為html，設定content type為text/html
            cout << "A Text request" << endl;
            strcpy(contentType, "text/html");
        }
        char Resource[4096]={0};
        if(strcmp(contentType, "video/mp4") == 0)//以是否為影片進行不同的處理
        {
            FILE *reader = fopen(obj.requestFilename, "rb+");//讀取影片檔案
            if(reader == NULL){
                cout << obj.requestFilename << ":does not exist" << endl;
                NOTFOND(fd);//檔案不存在，回傳404
                exit(1);     
            }
            else
            {
                //檔案存在，回傳200
                cout << obj.requestFilename << ":does  exist" << endl;
                // 以fseek獲取文件大小好方便回傳Content Length
                fseek(reader, 0L, SEEK_END);
                int fileLength = ftell(reader);
                fseek(reader, 0, SEEK_SET); //將讀寫位置設為檔案的開頭
                if(!obj.Range){  // 第一次先送header跟range
                    cout << "200 OK" << endl;
                    snprintf(Resource, 4096, "HTTP/1.1 200 OK\nContent-Type: %s\nContent-Length: %d\nAccept-Ranges: bytes\n\n", contentType, fileLength);
                    cout << Resource << endl;
                    send(fd, Resource, strlen(Resource), MSG_NOSIGNAL);
                    cout << "send header success" << endl;
                }
                else//以斷點續傳方式傳送後續影片，每次傳送部分片段
                {
                    long contentLeft = fileLength - obj.startPoint;//檔案剩餘大小
                    int fragSize = 65536;//response的檔案大小(64KB)，64*1024=65536
                    int fragNum = contentLeft / fragSize;//需要傳送的Response次數
                    if(contentLeft % fragSize != 0)
                    {
                        fragNum++;//多增加一次response以處理餘數
                    }
                    for(int i = 0; i < fragNum; i++)
                    {
                        if(i+1 == fragNum)
                        {
                            fragSize = fileLength - obj.startPoint; //將response大小重設以處理無法整除的剩餘部分
                        }
                        snprintf(Resource, 4096, "HTTP/1.1 206 Partial Content\nContent-Type: %s\nContent-Length: %d\nContent-Range: bytes %d-%d/%d\nAccept-Ranges: bytes\n\n",contentType, fragSize, obj.startPoint, obj.startPoint+fragSize-1, fileLength);
                        send(fd, Resource, strlen(Resource), MSG_NOSIGNAL);
                        char sizeBuffer[fragSize]={0};
                        fseek(reader, obj.startPoint, SEEK_SET);//將讀寫位置設為瀏覽器要求的位置
                        if(fread(sizeBuffer, 1, fragSize, reader) == 0){
                            cout << "read video file error." << endl;
                            exit(1);
                        }
                        else
                        {
                            send(fd, sizeBuffer, fragSize, MSG_NOSIGNAL);
                            obj.startPoint += fragSize;//在response後移動下次要傳送的讀寫位置
                            memset(sizeBuffer, 0, fragSize);//在每次寫入後清空buffer
                        }
                    }
                    cout << "Transfer video done" << endl;
                }
            }
        }
        else
        {
            FILE *reader = fopen(obj.requestFilename, "r");
            if(reader == NULL){
                cout << obj.requestFilename << ":does not exist" << endl;
                NOTFOND(fd);//檔案不存在，回傳404
                exit(1);     
            }
            else
            {
                cout << obj.requestFilename << ":does  exist" << endl;
                fseek(reader, 0L, SEEK_END);
                int fileLength = ftell(reader);//利用fseek取得檔案長度
                fseek(reader, 0, SEEK_SET);    //將讀寫位置設為檔案的開頭
                //檔案存在，回傳200
                snprintf(Resource, 4096, "HTTP/1.1 200 OK\nContent-Type: %s\nContent-Length: %d\nAccept-Ranges: bytes\n\n", contentType, fileLength);
                send(fd, Resource, strlen(Resource), MSG_NOSIGNAL);
                char siezBuffer[fileLength]={0};
                if(fread(siezBuffer, 1, fileLength, reader) == 0){
                    cout << "error.\n";
                    exit(1);
                }
                send(fd, siezBuffer, fileLength, 0);
            }
        }
    }
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
    serv_addr.sin_addr.s_addr = INADDR_ANY;//設定IP
    serv_addr.sin_port = htons(PORT_NUM);//設定port
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
        pid_t id = fork();
        if(id == -1){
            cout << "Fork Error.\n";//fork失敗，回傳-1，結束該程序
            return -1;
        }
        if(id == 0){   // 子程序，負責處理request
            close(listenfd);
            request_handler(socketfd);
            exit(0);//處理完畢，結束子程序
        }
        else if(id > 0){
            close(socketfd);
        }
    }    
    return 0;
}