#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { false, true }bool;
#define MAX_PATH_LEN 500
bool isDir(const char* path)//檢查是不是資料夾
{
	struct stat st;
    lstat(path, &st);
    return 0 != S_ISDIR(st.st_mode);
	//為資料夾的話return true，是檔案的話return false
}
void findFiles(const char *path)//遍歷當前資料夾
{
	unsigned long len;
    char temp[MAX_PATH_LEN];
    //去掉末尾的'/'
    len = strlen(path);
    strcpy(temp, path);
    if(temp[len - 1] == '/') temp[len -1] = '\0';
    
    if(isDir(temp))
    {
        //處理目錄
        int recursive = 1;
        RecursiveCheck(temp, recursive);//以遞迴處理子資料夾
    }
    else   //輸出檔案名
    {
        char *dest="wc -l ";
        strcat(dest,path);
        char *command;
        strcpy( command, dest );
        system(command);
        sprintf("wc -l %s\n", path);
    }
}
void RecursiveCheck(const char *path, int recursive)//遍歷子資料夾的遞迴函式
{
	DIR *pdir;
    struct dirent *pdirent;
    char temp[MAX_PATH_LEN];
    pdir = opendir(path);
    if(pdir)
    {
        while((pdirent = readdir(pdir)))
        {
            //跳過"."和".."
            if(strcmp(pdirent->d_name, ".") == 0
               || strcmp(pdirent->d_name, "..") == 0)
                continue;
            sprintf(temp, "%s/%s", path, pdirent->d_name);
            
            //當temp為資料夾並且recursive為1的時候繼續遞迴處理子資料夾
            if(isDir(temp) && recursive)
            {
                RecursiveCheck(temp, recursive);
            }
            else
            {
				//輸出檔案名
                // char *dest="wc -l ";
                // strcat(dest,temp);
                // char *command;
                // strcpy( command, dest );
                // system(command);
                sprintf("wc -l %s\n", temp);
            }
        }
    }
    else
    {
        printf("opendir error:%s\n", path);
    }
    closedir(pdir);
}
int main()
{
	char cwd[MAX_PATH_LEN];
	getcwd (cwd, MAX_PATH_LEN);
	findFiles(cwd);
	return 0;
}