#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define TOKEN_BUFSIZE 64
#define MAX_DIR_NAME 2048
#define TOK_WORD " \t\r\n\a"//由於getline會留下一個換行字符，因此除將字串移除空格外也要移除換行符號
int shell_build_in_cmd(char **args){
    if(strcmp(args[0],"exit")==0)
    {
        exit(0);
    }
    else if(strcmp(args[0],"cd")==0)
    {
        if(chdir(args[1]))
        {
            printf("cd %s:no such directory\n", args[1]);
        }
        return 1;
    }//用chdir來改變目錄
    else if(strcmp(args[0],"pwd")==0)
    {
        char buffer[MAX_DIR_NAME]={0};
        printf("%s\n", getcwd(buffer,sizeof(buffer)));
        return 1;
    }//用getcwd來確認目前所在的目錄
    else
    {
        return 0; //非以上三種內置指令，需要進行Fork才可執行，因此return 0
    }
}
char *shell_read_line()
{
    char *line = NULL;
    ssize_t bufsize = 0; // 利用 getline 來自動分配緩衝區，將整串輸入放入到line中
    getline(&line, &bufsize, stdin);
    return line;
};
char **shell_split_line(char *line){
    int bufsize = TOKEN_BUFSIZE, position = 0;
    char **commands = malloc(bufsize * sizeof(char*));//分配記憶體用以分析以空格分割的輸入，先假設有64個區段
    char *command;
    if (!commands) {
        fprintf(stderr, "shell allocation error\n");//若記憶體分配失敗則輸出錯誤資訊，並停止執行
        exit(1);
    }
    command = strtok(line, TOK_WORD);//利用strtok分離出指令的第一個詞，如cd final中的cd
    while (command != NULL) {
        commands[position] = command;//在commands[postion]儲存指令
        position++;
        if (position >= bufsize) {//如果片段太多，超過BUFFSIZE就繼續分配更多記憶體給commands
            bufsize += TOKEN_BUFSIZE;
            commands = realloc(commands, bufsize * sizeof(char*));//利用realloc在保留內容物的情況下增加記憶體空間
            if (!commands) {
                fprintf(stderr, "shell allocation error\n");//若command分配後反而為null則終止程式
                exit(1);
            }
        }
        command = strtok(NULL, TOK_WORD);//繼續分割字串的剩餘部分，由於getline會留下一個換行字符，因此除將字串移除空格外也要移除換行符號
    }
    commands[position] = NULL;//最後一位設成NULL
    return commands;
};
int shell_execute(char **args)
{
    if(args[0]==NULL)//如果沒有指令則跳過這輪的執行
    {
        return 1;
    }
    else if(shell_build_in_cmd(args))//是內建指令，直接執行
    {
        return 1;
    }
    else{       //由於外置指令需要重開進程才能執行，因此fork後再執行
        int pid=fork();
        if(pid==0)
        {
            if(execvp(args[0],args)<0)//執行外置指令
            {
                printf("%s:command not found.\n", args[0]);//指令不存在
                exit(0);
            }
        }
        else if(pid<0)
        {
            printf("fork error\n");
            exit(1);
        }
        wait(pid);
    }
    return 1;
};
void shell_loop()
{
    char *line;
    char **args;
    char buffer[MAX_DIR_NAME];
    while (1)
    {
        printf("B0829039's Shell:%s> ", getcwd(buffer, sizeof(buffer)));
        line = shell_read_line();//接收輸入的字串
        args = shell_split_line(line);//分割輸入的字串
        shell_execute(args);//執行指令
        free(line);//回收記憶體
        free(args);//回收記憶體
    }
};
int main()
{
    shell_loop();
    return 0;
}