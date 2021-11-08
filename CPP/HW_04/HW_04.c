#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_STRING_LEN 500
int main()
{
    FILE *fp;
    struct utmp user_rec;
    char user[8];
    char message[MAX_STRING_LEN];
    printf("the user online:\n");
    system("who");
    printf("The user you want to send message\n");
    scanf("%S", &user);
    fp = fopen("/var/run/utmp", "r");
    while(fread(&user_rec, sizeof(struct utmp),1,fp)>0)
    {
        if(user==user_rec.ut_user)
        {
            printf("The message you want to send\n");
        }
    }
    // printf("The message you want to send\n");
    // scanf("%S", &message);
    // printf("%S", message);
    return 0;
}