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
    char user[8];   
    printf("the user online:\n");
    system("who");
    printf("The user you want to send message\n");
    scanf("%S", &user);
    
    return 0;
}