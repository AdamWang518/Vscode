#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{ 
    char user[10];   
    //printf("the user online:\n");
    //system("who");
    printf("請輸入你想傳送訊息的對象：\n");
    scanf("%S", &user);
    printf("%S\n",user);
    return 0;
}