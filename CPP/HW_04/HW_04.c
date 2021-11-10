#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#define MAX_STRING_LEN 500
int main()
{
    FILE *fp;
    struct utmp user_rec;
    char user[8];
    char *line[15];
    char *line2=" > /dev/";
    strcpy(line, line2);
    char message[MAX_STRING_LEN];
    char *command[MAX_STRING_LEN];
    char *command2= "echo -e ";
    strcpy(command, command2);
    printf("the user online:\n");
    system("who");
    printf("The user you want to send message\n");
    scanf("%S", &user);
    fp = fopen("/var/run/utmp", "r");
    while(fread(&user_rec, sizeof(struct utmp),1,fp)>0)
    {
        if(user_rec.ut_user==user)
        {
            printf("Find it");
            printf("What message would you like to send?");
            scanf("\"%S\"", &message);
            strcat(line, user_rec.ut_line);
            strcat(message,line);
            strcat(command, message);
            system(command);
        }
        //printf("%s\t\t%s\n", user_rec.ut_user, user_rec.ut_line);
    }
    // printf("The message you want to send\n");
    // scanf("%S", &message);
    // printf("%S", message);
    return 0;
}